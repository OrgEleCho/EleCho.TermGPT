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
    std::string buf;
    std::deque<std::string> events;
    ThreadSafeDeque<std::string> event_quene;

  public:
    bool try_get_event(std::string &event)
    {
        while(event_quene.try_pop_front(event))
        {
            buf.append(event);
        }
        // 把buf按照\n\n分割，放到events里面，最后剩下的如果没有换行符，就放到buf里面
        size_t pos = 0;
        while((pos = buf.find("\n\n")) != std::string::npos)
        {
            events.push_back(buf.substr(0, pos));
            buf.erase(0, pos + 2);
        }

        if(events.size() > 0)
        {
            event = events.front();
            events.pop_front();
            return true;
        }
        return false;
    }

    virtual void run() = 0;
};
