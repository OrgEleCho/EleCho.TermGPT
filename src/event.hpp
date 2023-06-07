#pragma once
#include <cstdlib>
#include <string>
#include <ctime>
#include <format>
#include <iostream>
#include "common.hpp"
static inline std::string get_current_time()
{
    time_t timep;
    time(&timep);
    return std::string(ctime(&timep)).substr(0, 24);
    //TODO: 时间格式化 区域不完善
}
inline void error(std::string message)
{
    std::string time = get_current_time();
    std::cout << std::format("[{}][ERROR]{}{}", time, message, endl);
    exit(-1);
}
inline void warning(std::string message)
{
    std::string time = get_current_time();
    std::cout<<std::format("[{}][WARNING]{}{}", time, message,endl);
}
inline void debug(std::string message)
{
    std::string time = get_current_time();
    std::cout<<std::format("[{}][DEBUG]{}{}", time, message,endl);
}
