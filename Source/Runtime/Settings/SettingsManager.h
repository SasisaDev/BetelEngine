#pragma once

#include <map>
#include <string>
#include <concepts>
#include "INI/Ini.h"

class Settings;
class IPlatform;

template<class T>
concept SettingsDerived = std::is_base_of<Settings, T>::value;

class SettingsManager
{
protected:
    INIFile* settingsFile;
    std::map<std::string, Settings*> LoadedSettings;
public:
    SettingsManager() {
        // TODO: Load file
        char buffer[] = "[Test]\nKey=Value\0";
        size_t size = sizeof(buffer);


        settingsFile = INIFile::LoadFromMemory(buffer, size);
    }

    void SaveAll() {
        // TODO: Settings Saving Mechanism
        size_t bufSize = 0;
        settingsFile->GenerateFileBuffer(&bufSize);
        unsigned char* buf = new unsigned char[bufSize];
        settingsFile->GenerateFileBuffer(&bufSize, buf);

        // Write file
    }

    template <SettingsDerived SettingsType>
    SettingsType* GetOrDefault() {
        for(auto pair : LoadedSettings) {
            if(pair.first == SettingsType::GetName()) {
                return (SettingsType*) pair.second;
            }
        }
        
        SettingsType* defaultSettings = new SettingsType();
        if(settingsFile->HasDomain(SettingsType::GetName())) {
            defaultSettings->Deserialize(settingsFile->GetDomain(SettingsType::GetName()));
        }

        LoadedSettings.insert({SettingsType::GetName(), dynamic_cast<Settings*>(defaultSettings)});

        return defaultSettings;
    }
};