#include "config.hpp"
#include "event.hpp"
#include "openai/apis/models.hpp"
#include "openai/openaiapi.h"
#include "openai/program/chat_completion.hpp"
#include "openai/program/error.h"
#include <stdexcept>
#include <thread>
#include <unistd.h>
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
        std::cout << res << endl;
    }
    catch (const std::runtime_error &e)
    {
        error(std::format("[Openai]{}", e.what()));
    }
}
void test_chat()
{

    try
    {
        ChatCompletion cc;
        cc.GetCompletionWithStream(ChatHistory("简单介绍一下你自己，10个字之内"), ChatCompletionTypeModel::gpt_3_turbo);
        std::string s;
        while (1)
        {
            switch (cc.PullMessage(s))
            {

            case ChatCompletionStreamStatus::no_event:
                break;

            case ChatCompletionStreamStatus::event_passed:
                std::cout << s;
                break;
            case ChatCompletionStreamStatus::end:
                goto done;
                break;
            }
        }
    done:
        std::cout << endl;
    }
    catch (const std::runtime_error &e)
    {
        error(std::format("[Openai]{}", e.what()));
    }
    catch (ApiError e)
    {
        error(std::format("[Openai]{}\n{}", e.body, e.message));
    }
}
void tui()
{
    std::string input_buf;
    while (1)
    {
        std::cout << ">";
        std::cin >> input_buf;
        if (input_buf == "exit")
        {
            break;
        }
        else if (input_buf == "load_config")
        {
            load_config();
        }
        else if (input_buf == "test_chat")
        {
            test_chat();
        }
        else
        {
            std::cout << "未知命令" << endl;
        }
    }
}
int main(int argc, char **argv)
{
    extern Config config;
    tui();
    return 0;
}