#pragma once

#include <string>

class Settings 
{
protected:
    std::string filename;
public:
    Settings() = delete;
    Settings(const char* FileName);
};