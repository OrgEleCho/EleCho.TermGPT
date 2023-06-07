#include <iostream>
#pragma ocne
#include "openaiapi.h"
#include <string>
class BaseRoute
{
  protected:
    BaseRoute()
    {
        std::cout << "BaseRoute" << endl;
        if (OpenaiApi::is_inited() == false)
        {
            throw std::runtime_error("OpenaiApi is not inited");
        }
    }
};
class ApiGetRoute : public BaseRoute
{
  protected:
    std::string route;
    virtual std::string run() = 0;
};
class ApiPostRoute
{
  protected:
    std::string route;
    virtual std::string run() = 0;
};

class ApiPostRouteWithSSEResponse : public ApiPostRoute
{
};

// 可用的api

class OpenaiModels : public ApiGetRoute
{
  public:
    std::string run()
    {
        return OpenaiApi::get("/v1/models", std::map<std::string, std::string>());
    }
};
