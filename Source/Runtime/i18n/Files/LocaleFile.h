#pragma once

#include <string>

#include <Platform/Platform.h>

class LocaleFile 
{
    friend class TextManager;

    IFile* file;
    std::string localeID;
    std::string localeName;
public:

};