#pragma once
#include <string>
// TODO: 那么应当引导用户选择合适的上色方式

class Colorizer
{
  private:
    Color color;
    

  public:
    Colorizer(Color color) : color(color)
    {
    }
    std::string colorize(std::string content)
    {
        switch (color)
        {
        case Color::NONE: {
            return content;
        }
        }
        return content;
    };
};

