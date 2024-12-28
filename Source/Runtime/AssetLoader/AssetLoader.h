#pragma once

#include <map>
#include <unordered_set>
#include <vector>
#include <optional>
#include <string>

#include <Platform/Platform.h>

class AssetLoader
{
    friend class AssetGarbageCollector;
public:
    static AssetLoader& Get() {
        static AssetLoader loader;
        return loader;
    }
};