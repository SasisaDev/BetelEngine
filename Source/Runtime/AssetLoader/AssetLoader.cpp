#include "AssetLoader.h"

#include <Log/Logger.h>

Resource* AssetLoader::LoadResource(std::string path)
{
    // TODO: Make Archive Loading

    IFile* file = IPlatform::Get()->OpenContentFile(path, FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY | FILE_ACCESS_FLAG_ATE);
    if(!file->IsOpen())
    {
        LOGF(Error, LogAssetLoader, "Attempted to load Resource \"%s\", but it doesn't exist", path.c_str());
        return nullptr;
    }

    Resource* resource = new Resource(file->FetchAllBinary());

    LOGF(Log, LogAssetLoader, "Loaded Resource \"%s\".", path.c_str());

    return resource;
}