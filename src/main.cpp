#include "config.hpp"
#include "event.hpp"
#include "openai/apis/models.hpp"
#include "openai/openaiapi.h"
#include <stdexcept>
#include <thread>
#include <unistd.h>
#include "openai/apis/chat_completion.hpp"
void load_config()
{
    extern Config config;
    // Openai设置
    try
    {
        OpenaiApi::set_base_url(config.base_url);
        OpenaiApi::set_timeout(config.request_timeout);
        OpenaiApi::set_api_key(config.apikey);
    }
    catch (const std::runtime_error &e)
    {
        error(std::format("[Openai]{}", e.what()));
    }
    // 测试OpenaiApi连通性
    try
    {
        auto res = OpenaiApiModels().run();
        std::cout<<res<<endl;
    }
    catch (const std::runtime_error &e)
    {
        error(std::format("[Openai]{}", e.what()));
    }
    catch (...)
    {
        error("[Openai]未知错误");
    }
}
void test_chat()
{
    std::string body = "{\"model\": \"gpt-3.5-turbo\",\"messages\": [{\"role\": \"user\", \"content\": "
                       "\"简单的介绍一下你自己\"}],\"stream\":true}";
    try
    {
        auto j = OpenaiApiChatCompletionWithStream(body);
        j.run();
        while (1)
        {
            std::string e;
            if(j.try_get_event(e))
            {
                std::cout<<e<<endl;
            }
            else
            {
                std::cout<<"no event"<<endl;
            }
        }
    }
    catch (const std::runtime_error &e)
    {
        error(std::format("[Openai]{}", e.what()));
    }
    catch (...)
    {
        error("[Openai]未知错误");
    }
}
int main(int argc, char **argv)
{
    extern Config config;
    load_config();
    test_chat();
    return 0;
}