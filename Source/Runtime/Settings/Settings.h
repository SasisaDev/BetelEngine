#pragma once

#include <string>
#include <map>

class Settings 
{
    INIFile* file;
    friend class SettingsManager;
public:
    static std::string GetName() {return "Default";}

    virtual void Deserialize() {}
    virtual void Serialize() {}
};