#pragma once
#include "threadsafedeque.hpp"
#include <iostream>

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

class ApiPostRouteWithSSEResponse
{
  protected:
    ThreadSafeDeque<std::string> event_quene;

  public:
    bool try_get_event(std::string &event)
    {
        // 取出一个，如果以\n结尾，就是一个完整的event，否则继续取，直到取到\n结尾的event，剩下的放回去
        std::string temp;
        

        return event_quene.try_pop_front(event);
    }

    virtual void run() = 0;
};
