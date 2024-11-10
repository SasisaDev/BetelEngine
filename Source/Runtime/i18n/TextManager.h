#pragma once

#include "Files/LocaleFile.h"
#include <vector>

class TextManager
{
protected:
    std::vector<LocaleFile*> locales;
    std::string currentLocale = "en_US";
public:

    bool SetLocale(std::string locale) {
        // Check if this locale is present
        for(LocaleFile* file : locales) {
            if(file->localeID == locale) {
                currentLocale = locale;
                return true;
            }
        }
        return false;
    }

    std::string Fetch(std::string domain, std::string subdomain, std::string name) {
        LocaleFile* targetLocale = nullptr;

        for(LocaleFile* locale : locales) {
            if(locale->localeID == currentLocale) {
                targetLocale = locale;
                break;
            }
        }

        if(targetLocale == nullptr) {
            return domain + "." + subdomain + "." + name;
        }

        // TODO: Add translation search logic
        return domain + "." + subdomain + "." + name;
    }
};