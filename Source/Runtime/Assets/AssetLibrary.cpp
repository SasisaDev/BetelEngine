#include "AssetLibrary.h"
#include <Platform/Platform.h>
#include <Log/Logger.h>

AssetLibrary& AssetLibrary::Get()
{
    static AssetLibrary library;
    return library;
}

void AssetLibrary::CrawlAssetsTyped(AssetType type)
{
    IPlatform* platform = IPlatform::Get();

    IDirectory* directory = platform->OpenDirectory("./Content");
    if(!directory->Exists()) {
        LOG(Warning, LogAsset, "Content directory does not exist. Are assets cooked?");
        return;
    }

    for(IDirectory* child : directory->GetChildren())
    {
        if(child->IsDirectory())
        {
            // Make path analysis
        }
    }
}

void AssetLibrary::CrawlAssetsAll()
{
    // TODO
    CrawlAssetsTyped(AssetType());
}