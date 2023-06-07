#pragma once
#include "../route.h"

class OpenaiApiModels : public ApiGetRoute
{
  public:
    std::string run() override
    {
        return OpenaiApi::get("/v1/models", std::map<std::string, std::string>());
    }
};

