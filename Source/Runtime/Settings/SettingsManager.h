#pragma once

#include <unordered_set>

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