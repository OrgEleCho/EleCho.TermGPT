#pragma once

#if defined(_WIN32) || defined(_WIN64)

#define endl "\r\n"

#elif defined(__linux__)

#define endl "\n"

#endif
#include <CJsonObject.hpp>
#include <string>
#include <map>

inline std::string SimpleStringMapToJson(std::map<std::string, std::string> input)
{
    neb::CJsonObject json;
    for(auto it = input.begin(); it != input.end(); it++)
    {
        json.Add(it->first, it->second);
    }
    return json.ToString();
}

