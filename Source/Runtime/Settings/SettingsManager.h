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
    std::map<std::string, Settings*> LoadedSettings;
public:
    SettingsManager() {
        // Test
        INIFile settingsFile;
        char buffer[] = "[Test]\nKey=Value\\\nMultiline!\0";
        size_t size = sizeof(buffer);

        settingsFile = INIFile::LoadFromMemory(buffer, size);

        std::string outbuffer = settingsFile.GenerateFileBuffer();

        // Create Config Directory if it doesn't exist yet
        IPlatform::Get()->OpenLocalDirectory("./Config/", EDirectoryFlags::DIRECTORY_FLAG_CREATE);
    }

    void SaveAll() {
        // TODO: Settings Saving Mechanism
        

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
        IFile* settingsFile = IPlatform::Get()->OpenLocalFile("./Config/" + SettingsType::GetName() + ".ini", EFileAccessFlags::FILE_ACCESS_FLAG_READ | EFileAccessFlags::FILE_ACCESS_FLAG_BINARY | EFileAccessFlags::FILE_ACCESS_FLAG_ATE);
        if(!settingsFile || !settingsFile->IsOpen()) {
            LOGF(Warning, LogSettings, "Settings file for \"%s\" was not find. Creating new one", SettingsType::GetName().c_str());

            IFile* settingsFileWriter = IPlatform::Get()->OpenLocalFile("./Config/" + SettingsType::GetName() + ".ini", EFileAccessFlags::FILE_ACCESS_FLAG_WRITE);
            INIFile defaultINI = defaultSettings->Serialize();
            settingsFileWriter->Write(defaultINI.GenerateFileBuffer());
            settingsFileWriter->Close();
        }

        std::vector<char> settingsFileData = settingsFile->FetchAllBinary();
        INIFile ini = INIFile::LoadFromMemory(settingsFileData.data(), settingsFileData.size());
        defaultSettings->Deserialize(ini);

        LoadedSettings.insert({SettingsType::GetName(), dynamic_cast<Settings*>(defaultSettings)});

        settingsFile->Close();
        return defaultSettings;
    }
};