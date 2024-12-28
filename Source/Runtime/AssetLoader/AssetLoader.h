#pragma once

#include <cstdint>
#include <map>
#include <unordered_set>
#include <vector>
#include <optional>
#include <string>

#include <Platform/Platform.h>

#include "Objects/BlameMaster.h"
#include "Resources/Archive.h"

#include <Resources/Resource.h>

class AssetLoader
{
    friend class AssetGarbageCollector;
protected:
    std::vector<BlameMasterFile*> blameMasters;
    std::vector<ArchiveFile*> resourceArchives;
public:
    static AssetLoader& Get() {
        static AssetLoader loader;
        return loader;
    }

    /* 
     * Loads Asset from system's File System or Archive File
     *
     * Returns nullptr if resource doesn't exist
    */
    Resource* LoadResource(std::string path);
};