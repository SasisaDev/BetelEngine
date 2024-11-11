#pragma once

#include "Files/LocaleFile.h"
#include <vector>

class TextManager
{
protected:
    std::vector<LocaleFile*> locales;
    std::string currentLocale = "en_US";
public:

    const std::vector<LocaleFile*>& GetLocales() const {return locales;}

    void Initialize() {
        // Fetch Current Locale

        IDirectory* gameDir = IPlatform::Get()->OpenDirectory("./Content/i18n/");
        IDirectory* editorDir = nullptr;
        #ifdef EDITOR
        editorDir = IPlatform::Get()->OpenLocalDirectory("Editor/Content/i18n/");
        #endif

        for(IDirectory* dir : gameDir->GetChildren()) {
            if(dir->IsDirectory()) continue;

            if(IFile* file = IPlatform::Get()->OpenFile(dir->GetPath(), FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY)) {
                LOGF(Log, LogI18N, "Found translation file: %s", dir->GetPath().GetPath().c_str());

                LocaleFile* locale = new LocaleFile();
                locale->Load({file});
                locale->localeID = dir->GetPath().GetName();
                locales.push_back(locale);
            }
        }
    }

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
        return targetLocale->Get( domain + "." + subdomain + "." + name);
    }
};