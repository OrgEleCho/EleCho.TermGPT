#pragma once
#include "../route.h"
#include "../../common.hpp"
#include <thread>
class OpenaiApiChatCompletionWithStream : public ApiPostRouteWithSSEResponse
{
  private:
    std::string body_to_send;
    std::thread *t=nullptr;
  public:
    OpenaiApiChatCompletionWithStream(std::string body) : body_to_send(body)
    {
    }

    void run()override
    {
        t=new std::thread(
            [this]() { OpenaiApi::post_with_sse_response("/v1/chat/completions", body_to_send, event_quene); });

        return;
    }
    ~OpenaiApiChatCompletionWithStream()
    {

        if(t != nullptr)
        {
            t->join();
            delete t;
        }
    }
};