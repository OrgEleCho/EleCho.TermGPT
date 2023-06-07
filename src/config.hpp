#pragma once
#include <iostream>
#include <string>
#include <inifile.h>
// 用于文件读写的头文件
#include <fstream>
#include <string>
#include <format>
#include <array>
#include <sys/stat.h>
#include "event.h"
#include "event.hpp"
const std::array<std::string,1> where_is_config_file = {
    "./config.ini",
};

class Config
{
    public:
    std::string base_url = "https://openaiapi.elecho.org/";
    std::string apikey = "sk-put-your-api-key-here";
    std::string colorize = "none";
    int request_timeout = 10;
    std::string config_file_path = "";
    inifile::IniFile config_file;
    bool save_on_changed=false;
    Config()
    {

        for (auto &i : where_is_config_file)
        {
            struct stat buffer;
            if (stat(i.c_str(), &buffer) == 0)
            {
                // 文件存在
                config_file_path = i;
                int res = config_file.Load(config_file_path);
                if (res != 0)
                {
                    error(std::format("[配置文件]文件{}解析错误:{}", config_file_path,std::to_string(res)));
                }
                else
                {
                    res = config_file.GetStringValue("openai", "base_url", &base_url);
                    res = config_file.GetStringValue("openai", "apikey", &apikey);
                    res = config_file.GetIntValue("openai", "request_timeout", &request_timeout);
                    res = config_file.GetStringValue("custom", "colorize", &colorize);
                    if(res != 0)
                    {
                        error(std::format("[配置文件]文件{}解析错误:{}", config_file_path,std::to_string(res)));
                    }
                    // 输出配置信息 debug
                    debug(std::format("[配置文件]openai.base_url:{}", base_url));
                    debug(std::format("[配置文件]openai.apikey:{}", apikey));
                    debug(std::format("[配置文件]openai.request_timeout:{}", request_timeout));
                    debug(std::format("[配置文件]custom.colorize:{}", colorize));
                    


                }
            }
        }
        if (config_file_path == "")
        {
            config_file_path = where_is_config_file[0];
            create_new();
        }
    }
    void change(std::string section,std::string key, std::string value)
    {
        config_file.SetStringValue(section, key, value);
        debug(std::format("[配置文件]{}:{}={}", section, key, value));
        if(save_on_changed)
        {
            save();
        }
    }
    void create_new()
    {
        config_file.SetStringValue("openai", "base_url", base_url);
        config_file.SetStringValue("openai", "apikey", apikey);
        config_file.SetStringValue("openai", "request_timeout", std::to_string(request_timeout));
        config_file.SetStringValue("custom", "colorize", colorize);
    }
    void save()
    {
        int res = config_file.SaveAs(config_file_path);
        if (res != 0)
        {
            error(std::format("[配置文件]文件{}保存错误:{}", config_file_path,std::to_string(res)));
        }
        else
        {
            debug(std::format("[配置文件]文件{}保存成功", config_file_path));
        }
    }
    ~Config()
    {
        save();
    }
};
extern Config config;