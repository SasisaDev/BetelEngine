#pragma once

#include <map>
#include <string>
#include <concepts>

class Settings;
class IPlatform;

template<class T>
concept SettingsDerived = std::is_base_of<Settings, T>::value;

class SettingsManager
{
protected:
    std::map<std::string, Settings*> LoadedSettings;
public:
    SettingsManager(){}

    void LoadAll(){}

    template <SettingsDerived SettingsType>
    SettingsType* GetOrDefault() {
        for(auto pair : LoadedSettings) {
            if(pair.first == SettingsType::GetName()) {
                return (SettingsType*) pair.second;
            }
        }
        
        SettingsType* defaultSettings = new SettingsType();

        LoadedSettings.insert({SettingsType::GetName(), dynamic_cast<Settings*>(defaultSettings)});

        return defaultSettings;
    }

    void SaveAll(){}
};