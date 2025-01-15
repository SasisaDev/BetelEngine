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
#include <Object/Object.h>

struct ConvertChar 
{
    static uint32_t ToUInt32(char* buffer);
    static uint32_t ToUInt16(char* buffer);
    static uint32_t ToUInt8(char* buffer);
};

class AssetLoader
{
    friend class AssetGarbageCollector;

    BlameMasterFile* ParseBlameMasterFile(std::unique_ptr<IFile> file);
protected:
    std::vector<BlameMasterFile*> blameMasters;
    std::vector<ArchiveFile*> resourceArchives;
public:
    static AssetLoader& Get() {
        static AssetLoader loader;
        return loader;
    }

    void CrawlContent(std::string Path = "./Content");

    BlameMasterFileObjectContainer ReadObject(BlameMasterFile* master, uint32_t offset);

    /* 
     * Loads Object from Blame File, applying Plugin changes
     *
     * Returns nullptr if object doesn't exist
    */
    Object* LoadObject(uint32_t ObjectID);

    /* 
     * Loads Asset from system's File System or Archive File
     *
     * Returns nullptr if resource doesn't exist
    */
    Resource* LoadResource(std::string path);
};