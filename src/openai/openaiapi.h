#pragma once
#include "connection.h"
#include <deque>
// 封装出可以直接调用的网络请求接口，上层被route调用，下层调用connection即webclient
//这是一个静态类
class OpenaiApi
{
  public:
    static void set_base_url(std::string host);
    static void set_timeout(int timeout);
    static void set_api_key(std::string api_key);
    static std::string get(std::string route, std::map<std::string, std::string> params);
    static std::string post(std::string route, std::string body);
    static void post_with_sse_response(std::string route, std::string body, std::deque<std::string> &event_queue);
    static bool is_inited();

  private:
    static std::byte inited;
    static std::string base_url;
    static int timeout;
    static std::string api_key;
    static std::map<std::string, std::string> headers;
    
};