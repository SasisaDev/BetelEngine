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
    LocaleFile* GetLocale(std::string locale) {
        for(LocaleFile* locFile : locales) {
            if(locFile->localeID == locale) { 
                return locFile;
            }
        }
        return nullptr;
    }

    void Initialize() {
        // Fetch Current Locale

        IDirectory* gameDir = IPlatform::Get()->OpenContentDirectory("Game/i18n/");
        IDirectory* editorDir = nullptr;
#       ifdef EDITOR
        editorDir = IPlatform::Get()->OpenContentDirectory("Editor/i18n/");
#       endif

        for(IDirectory* dir : gameDir->GetChildren()) {
            if(dir->IsDirectory()) continue;

            if(IFile* file = IPlatform::Get()->OpenFile(dir->GetPath(), FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY)) {
                LOGF(Log, LogI18N, "Found translation file: %s", dir->GetPath().GetPath().c_str());

                std::vector<IFile*> files {file};

                // Load Editor Translations if we're in Editor Mode
                if(editorDir != nullptr) {
                    for(IDirectory* edDir : editorDir->GetChildren()) {
                        if(edDir->GetPath().GetName() == dir->GetPath().GetName()) {
                            LOGF(Log, LogI18N, "Found editor translation file: %s", edDir->GetPath().GetPath().c_str());
                            files.push_back(IPlatform::Get()->OpenFile(edDir->GetPath(), FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY));
                        }
                    }
                }

                LocaleFile* locale = new LocaleFile();
                locale->Load(files);
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