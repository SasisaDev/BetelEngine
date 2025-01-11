#include "AssetLoader.h"

#include <Log/Logger.h>
#include <Platform/Platform.h>

BlameMasterFile* AssetLoader::ParseBlameMasterFile(IFile* file)
{
    BlameMasterFile* BMF = new BlameMasterFile;
    BMF->SetName(file->GetPath().GetName()); 
    BMF->SetPath(file->GetPath().GetPath()); 
    BMF->SetMounted(true);

    // TODO: Fetch header

    // TODO: Fetch table

    return BMF;
}

void AssetLoader::CrawlContent(std::string Path)
{
    IDirectory* directory = IPlatform::Get()->OpenLocalDirectory(Path);

    if(!directory->Exists()) {
        LOGF(Error, LogAssetLoader, "Attempted to crawl directory \"%s\", but it doesn't exist.", Path.c_str());
        delete directory;
        return;
    }

    for(IDirectory* child : directory->GetChildren())
    {
        if(child->IsDirectory() == false)
        {
            // If file is a BMF file, parse it accoardingly
            if(child->GetPath().GetNameExt() == BLAME_MASTER_FILE_EXT) 
            {
                blameMasters.push_back(ParseBlameMasterFile(IPlatform::Get()->OpenFile(child->GetPath(), FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY)));
            }
        }
    }

    delete directory;
}

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