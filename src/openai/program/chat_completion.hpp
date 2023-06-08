#include "../apis/chat_completion.hpp"
#include "error.h"
#include <utility>
#include <CJsonObject.hpp>
enum class ChatRole
{
    system,
    user,
    assiatant
};
struct ChatMessage
{
    ChatRole role;
    std::string content;
    std::string name;
};
enum class ChatCompletionStreamStatus
{
    no_event,
    event_passed,
    end
};
enum class ChatCompletionTypeModel
{
    gpt_3_turbo,
    
};
class ChatHistory
{
  public:
    std::vector<ChatMessage> chat_histroy;
    ChatHistory(std::string prompt)
    {
        chat_histroy.push_back({ChatRole::user,prompt,""});
    }
    ChatHistory(std::string prompt,std::string system_message)
    {
        chat_histroy.push_back({ChatRole::system, system_message, ""});
        chat_histroy.push_back({ChatRole::user, prompt, ""});
    }
    void AddUserMessage(std::string message)
    {
        chat_histroy.push_back({ChatRole::user, message, ""});
    }
    void AddAssistantMessage(std::string message)
    {
        chat_histroy.push_back({ChatRole::assiatant, message, ""});
    }
};

class ChatCompletion
{
  private:
    OpenaiApiChatCompletionWithStream *api = nullptr;
    std::string jsonBuilder(ChatHistory chat_histroy, ChatCompletionTypeModel model,bool stream)
    {
        std::string model_name;
        switch (model)
        {
        case ChatCompletionTypeModel::gpt_3_turbo:
            model_name = "gpt-3.5-turbo";
            break;
        default:
            throw std::runtime_error("不支持的模型");
        }
        neb::CJsonObject json;
        json.Add("model", model_name);
        json.AddEmptySubArray("messages");

        for (auto &i : chat_histroy.chat_histroy)
        {
            neb::CJsonObject message;
            switch (i.role)
            {
            case ChatRole::user:
                message.Add("role", "user");
                break;
            case ChatRole::assiatant:
                message.Add("role", "assistant");
                break;
            case ChatRole::system:
                message.Add("role", "system");
                break;
            }
            message.Add("content", i.content);
            json["messages"].Add(message);
        }
        json.Add("stream", stream, true);
        return json.ToString();
    }

  public:
    std::string GetCompletion(ChatHistory chat_history, ChatCompletionTypeModel model)
    {
        //暂时不可用
        return "";
    }
    void GetCompletionWithStream(ChatHistory chat_history,ChatCompletionTypeModel model)
    {
        if(api != nullptr)
        {
            delete api;
        }
        api = new OpenaiApiChatCompletionWithStream(jsonBuilder(chat_history, model,true));
        api->run();
    }

    ChatCompletionStreamStatus PullMessage(std::string &s)
    {
        if(api == nullptr)
        {
            throw std::runtime_error("没有发送请求就调用了PullMessage");
        }
        std::string res;
        api->try_get_event(res);

        if(res == "")
        {
            return ChatCompletionStreamStatus::no_event;
        }
        // 如果不为空，那么正常的是以"data: "开头的，先检查，如果有就去掉，如果没有就报错
        if(res.substr(0,6) == "data: ")
        {
            res = res.substr(6);
        }
        else
        {
            throw ApiError{"api返回的数据不是以data:开头",res};
        }
        // 如果内容是[DONE]，那么就是结束了
        if(res == "[DONE]")
        {
            return ChatCompletionStreamStatus::end;
        }
        // 如果内容是json，且choices[0]delta不为空
        neb::CJsonObject json(res);
        if(json.KeyExist("choices"))
        {
            neb::CJsonObject choices = json["choices"];
            if(choices.GetArraySize() == 0)
            {
                throw ApiError{"api返回的数据choices为空",res};
            }
            else
            {
                if(choices[0].KeyExist("delta"))
                {
                    // 如果delta存在content
                    if(choices[0]["delta"].KeyExist("content"))
                    {
                        s = choices[0]["delta"]["content"].ToString().substr(1, choices[0]["delta"]["content"].ToString().length() - 2);
                        return ChatCompletionStreamStatus::event_passed;
                    }
                    else
                    {
                        return ChatCompletionStreamStatus::no_event;
                    }
                }
                else
                {
                    throw  ApiError{"api返回的数据choices[0]没有delta",res};
                }
            }
        }

        else
        {
            throw ApiError{"api返回的数据没有choices",res};
        }


    }
    ~ChatCompletion()
    {
        if(api != nullptr)
        {
            delete api;
        }
    }
};