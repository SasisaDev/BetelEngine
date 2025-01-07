#pragma once

#include <string>
#include <map>

#include "INI/Ini.h"

class Settings 
{
protected:
    INIFile* file = nullptr;
    friend class SettingsManager;
public:
    Settings(){}
    ~Settings(){if(file != nullptr){delete file; file = nullptr;}}

    static std::string GetName() {return "Default";}

    inline INIFile* GetIniFile() {return file;}

    virtual void Deserialize(INIFile& file) {}
    virtual INIFile Serialize() {return {};}
};