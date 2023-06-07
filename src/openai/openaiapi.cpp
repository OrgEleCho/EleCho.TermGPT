#include "openaiapi.h"
#include "threadsafedeque.hpp"
#include <memory>
#include <string>
#include <thread>

// static成员变量的定义
std::string OpenaiApi::base_url;
int OpenaiApi::timeout;
std::string OpenaiApi::api_key;
std::map<std::string, std::string> OpenaiApi::headers;
std::byte OpenaiApi::inited{0x00};

void OpenaiApi::set_base_url(std::string h)
{
    base_url = h;
    // 检测是否http(s)//开头
    if (base_url.find("http://") != 0 && base_url.find("https://") != 0)
    {
        throw std::runtime_error("base_url must start with http:// or https://");
    }
    // 如果结尾有/，则去掉
    if (base_url[base_url.length() - 1] == '/')
    {
        base_url = base_url.substr(0, base_url.length() - 1);
    }
    inited |= std::byte{0b10000000};
}
void OpenaiApi::set_timeout(int t)
{
    // 0或或者负数都是不合法的
    if (t <= 0)
    {
        throw std::runtime_error("timeout must be greater than 0");
    }
    timeout = t;
    inited |= std::byte{0b01000000};
}
void OpenaiApi::set_api_key(std::string k)
{
    // sk-开头,后面48位字符串
    if (k.find("sk-") != 0 || k.length() != 51)
    {
        throw std::runtime_error("api_key must be a valid secret key");
    }
    api_key = k;
    headers["Authorization"] = "Bearer " + api_key;
    headers["Content-Type"] = "application/json";
    inited |= std::byte{0b00100000};
}
std::string OpenaiApi::get(std::string route, std::map<std::string, std::string> params)
{
    std::string url = base_url + route;
    std::string response;
    bool result = webclient.Get(
        url, headers, params, [&](const char *data, size_t len) { response.append(data, len); }, timeout);
    if (!result)
    {
        throw std::runtime_error("get request failed");
    }
    return response;
}
std::string OpenaiApi::post(std::string route, std::string body)
{
    std::string url = base_url + route;
    std::string response;
    bool result = webclient.Post(
        url, headers, body, [&](const char *data, size_t len) { response.append(data, len); }, timeout);
    if (!result)
    {
        throw std::runtime_error("post request failed");
    }
    return response;
}
void OpenaiApi::post_with_sse_response(std::string route, std::string body, ThreadSafeDeque<std::string> &event_queue)
{
    std::string url = base_url + route;
    std::string buf;
    bool result = webclient.Post(
        url, headers, body,
        [&](const char *data, size_t len) {
            buf.append(data, len);
        },
        timeout);
    if (!result)
    {
        throw std::runtime_error("post request failed");
    }
}
bool OpenaiApi::is_inited()
{
    return inited == std::byte{0b11100000};
}