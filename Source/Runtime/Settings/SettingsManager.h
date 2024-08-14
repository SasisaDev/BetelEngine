#pragma once

#include <unordered_multiset>

class Settings;

class SettingsManager
{
protected:
    std::unordered_multiset<Settings*> LoadedSettings;
public:
    Settings* Load();
    Settings* Get();
    Settings* GetOrDefault();
};