#pragma once

#include <string>
#include <map>

class Settings 
{
public:
    static std::string GetName() {return "Default";}

    virtual void Deserialize(std::map<std::string, std::string> data) {}
    virtual std::map<std::string, std::string> Serialize() {return {};}
};