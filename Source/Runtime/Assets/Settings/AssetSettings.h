#pragma once

#include <Settings/Settings.h>

class AssetSettings : public Settings
{
public:
    static std::string GetName() {return "Assets"; }

    bool EnableGarbageCollector = true;
    // Time before each GC pass
    float GCCycleTime = 30.f;
};