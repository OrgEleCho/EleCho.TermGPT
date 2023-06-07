#pragma once
#include "../route.h"
#include "../../common.hpp"
class OpenaiApiChatCompletionWithStream : public ApiPostRouteWithSSEResponse
{
  private:
    std::string body_to_send;
  public:
    OpenaiApiChatCompletionWithStream(std::string body) : body_to_send(body)
    {
    }

    void run()override
    {
        return OpenaiApi::post_with_sse_response("/v1/chat/completions", body_to_send, event_quene);
    }
};