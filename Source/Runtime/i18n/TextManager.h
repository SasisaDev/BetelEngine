#pragma once

#include "Files/LocaleFile.h"
#include <vector>

class TextManager
{
protected:
    std::vector<std::unique_ptr<LocaleFile>> locales;
    LocaleFile* targetLocale;
    std::string currentLocale = "en_US";
public:

    const std::vector<std::unique_ptr<LocaleFile>>& GetLocales() const {return locales;}
    LocaleFile* GetLocale(std::string locale) {
        for(std::unique_ptr<LocaleFile>& locFile : locales) {
            if(locFile->localeID == locale) { 
                return locFile.get();
            }
        }
        return nullptr;
    }

    void Initialize() {
        // Fetch Current Locale

        std::unique_ptr<IDirectory> gameDir = IPlatform::Get()->OpenContentDirectory("i18n/");
        std::unique_ptr<IDirectory> editorDir = nullptr;
#       ifdef EDITOR
        editorDir = IPlatform::Get()->OpenContentDirectory("Editor/i18n/");
#       endif

        if(gameDir.get() == nullptr || !gameDir->Exists()) {
            LOG(Error, LogI18N, "Could not find translation files");
            return;
        }

        for(IDirectory* dir : gameDir->GetChildren()) {
            if(dir->IsDirectory()) continue;

            std::vector<IFile*> files;
            files.reserve(2);
            files.push_back(IPlatform::Get()->OpenFile(dir->GetPath(), FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY).release());
            if(files[0]) {
                LOGF(Log, LogI18N, "Found translation file: %s", dir->GetPath().GetPath().c_str());

                // Load Editor Translations if we're in Editor Mode
                if(editorDir != nullptr) {
                    for(IDirectory* edDir : editorDir->GetChildren()) {
                        if(edDir->GetPath().GetName() == dir->GetPath().GetName()) {
                            LOGF(Log, LogI18N, "Found editor translation file: %s", edDir->GetPath().GetPath().c_str());
                            files.push_back(IPlatform::Get()->OpenFile(edDir->GetPath(), FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY).release());
                            break;
                        }
                    }
                }

                LocaleFile* locale = new LocaleFile();
                locale->Load(files);
                locale->localeID = dir->GetPath().GetName();
                locales.emplace_back(std::move(locale));
            }
        }
    }

    bool SetLocale(std::string locale) {
        // Check if this locale is present
        for(std::unique_ptr<LocaleFile>& file : locales) {
            if(file->localeID == locale) {
                currentLocale = locale;
                return true;
            }
        }
        return false;
    }

    std::string Fetch(const std::string_view& id) {
        for(std::unique_ptr<LocaleFile>& locale : locales) {
            if(locale->localeID == currentLocale) {
                targetLocale = locale.get();
                break;
            }
        }

        if(targetLocale == nullptr) {
            return id.data();
        }

        // TODO: Add translation search logic
        return targetLocale->Get(id.data());
    }
};