#pragma once

#include <vector>
#include <string>
#include <optional>

struct Argument
{
    std::string Key;
    std::string Value;

    Argument(std::string K, std::string V){Key = K; Value = V;}
};

struct ArgumentCollection
{
    void Push(Argument arg) 
    {
        Arguments.push_back(arg);
    }

    bool Has(std::string Key)
    {
        for(Argument& arg : Arguments)
        {
            if(arg.Key == Key)
            {
                return true;
            }
        }
        return false;
    }

    std::string Get(std::string Key) 
    {
        for(Argument& arg : Arguments)
        {
            if(arg.Key == Key)
            {
                return arg.Value;
            }
        }
        return {};
    }

    std::optional<std::string> GetOpt(std::string Key)
    {
        for(Argument& arg : Arguments)
        {
            if(arg.Key == Key)
            {
                return arg.Value;
            }
        }
        return {};
    }

    std::string GetOrDefault(std::string Key, std::string Default) 
    {
        for(Argument& arg : Arguments)
        {
            if(arg.Key == Key)
            {
                return arg.Value;
            }
        }
        return Default;
    }

private:
    std::vector<Argument> Arguments;
};

class ArgumentParser
{
public:
    static ArgumentCollection ParseArgs(int argc, char* argv[]);
};