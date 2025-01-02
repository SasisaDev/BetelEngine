#pragma once

#include <string>
#include <map>

class Settings 
{
protected:
    INIFile* file;
    friend class SettingsManager;
public:
    Settings(){}

    static std::string GetName() {return "Default";}

    virtual void Deserialize(INIFile& file) {}
    virtual INIFile Serialize() {return {};}
};