#include "config.hpp"
#include "event.hpp"
#include "openai/openaiapi.h"
#include "openai/route.h"
#include <stdexcept>
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
        auto res = OpenaiModels().run();
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
int main(int argc, char **argv)
{
    extern Config config;
    load_config();
    return 0;
}