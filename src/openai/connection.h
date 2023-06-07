#pragma once
#include "../event.hpp"
#include <cstring>
#include <curl/curl.h>
#include <functional>
#include <map>
#include <string>

class WebClient
{
  public:
    WebClient()
    {
        CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
        if (res != CURLE_OK)
        {
            throw std::runtime_error("Failed to init libcurl");
        }
        curl_handle_ = curl_easy_init();
        if (!curl_handle_)
        {
            throw std::runtime_error("Failed to init curl handle");
        }
    }

    ~WebClient() noexcept
    {
        try
        {
            if (curl_handle_)
            {
                curl_easy_cleanup(curl_handle_);
            }
            curl_global_cleanup();
        }
        catch (...)
        {
            // do nothing
        }
    }

    bool Get(const std::string &url, const std::map<std::string, std::string> &headers,
             const std::map<std::string, std::string> &params,
             const std::function<void(const char *, size_t)> &content_receiver, long timeout = 10)
    {
        std::string full_url = url + "?" + BuildQueryString(params);
        return PerformRequest(full_url, headers, nullptr, content_receiver, timeout);
    }

    bool Post(const std::string &url, const std::map<std::string, std::string> &headers, const std::string &body,
              const std::function<void(const char *, size_t)> &content_receiver, long timeout = 10)
    {
        return PerformRequest(url, headers, body.c_str(), content_receiver, timeout);
    }

  private:
    CURL *curl_handle_ = nullptr;

    bool PerformRequest(const std::string &url, const std::map<std::string, std::string> &headers, const char *body,
                        const std::function<void(const char *, size_t)> &content_receiver, long timeout)
    {
        CURLcode res = CURLE_OK;
        std::string err_msg;
        curl_easy_setopt(curl_handle_, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_handle_, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &content_receiver);
        curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT, timeout);
        curl_easy_setopt(curl_handle_, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl_handle_, CURLOPT_SSL_VERIFYHOST, 0L);

        struct curl_slist *header_list = nullptr;
        try
        {
            for (auto &header : headers)
            {
                std::string header_str = header.first + ": " + header.second;
                header_list = curl_slist_append(header_list, header_str.c_str());
                if (!header_list)
                {
                    throw std::runtime_error("Failed to add header: " + header_str);
                }
            }
            curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, header_list);

            if (body)
            {
                curl_easy_setopt(curl_handle_, CURLOPT_POST, 1L);
                curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDS, body);
                curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDSIZE, static_cast<long>(body ? strlen(body) : 0));
            }
            res = curl_easy_perform(curl_handle_);
        }
        catch (std::exception &e)
        {
            err_msg = e.what();
            if (header_list)
            {
                curl_slist_free_all(header_list);
            }
            throw;
        }

        if (header_list)
        {
            curl_slist_free_all(header_list);
        }

        if (res != CURLE_OK)
        {
            err_msg = "Failed to perform request: " + std::string(curl_easy_strerror(res));
        }

        if (!err_msg.empty())
        {
            std::string request_type = (body != nullptr) ? "POST" : "GET";
            std::string debug_info = "[WebClient] " + request_type + " " + url + " with headers:\n";
            for (auto &header : headers)
            {
                debug_info += "[WebClient]    " + header.first + "=" + header.second + "\n";
            }
            if (body)
            {
                debug_info += "[WebClient] with body: " + std::string(body) + "\n";
            }
            debug_info += "[WebClient] error: " + err_msg + "\n";
            throw std::runtime_error(debug_info);
        }

        return true;
    }

    static size_t WriteCallback(char *buffer, size_t size, size_t nitems, void *userp)
    {
        auto &content_receiver = *static_cast<std::function<void(const char *, size_t)> *>(userp);
        size_t total_size = size * nitems;
        content_receiver(buffer, total_size);
        return total_size;
    }

    static std::string BuildQueryString(const std::map<std::string, std::string> &params)
    {
        std::string result;
        for (auto &param : params)
        {
            if (!result.empty())
            {
                result += "&";
            }
            result += param.first + "=" + param.second;
        }
        return result;
    }
};
extern WebClient webclient;