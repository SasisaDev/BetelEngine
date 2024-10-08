#include "AssetLibrary.h"
#include <Platform/Platform.h>
#include <Log/Logger.h>
#include "AssetFile/AssetFile.h"

AssetLibrary& AssetLibrary::Get()
{
    static AssetLibrary library;
    return library;
}

Asset* AssetLibrary::LoadAsset(std::string Path)
{
    if(Asset* asset = GetAsset(Path)) {
        return asset;
    }

    AssetDescriptor* knownDescriptor = nullptr;

    for(auto desc : AssetList) {
        if(desc.Path == Path) {
            knownDescriptor = &desc;
        }
    }

    AssetFile assetFile(Path);
    assetFile.ReadFromDevice();

    Artifact assetArtifact;
    assetFile >> assetArtifact;

    std::string assetTypeName = assetFile.GetAssetTypeName();
    AssetType* assetType = nullptr;

    if(knownDescriptor) {
        assetType = knownDescriptor->Type;
    } else {
        for(AssetType* type : RegisteredAssetTypes) {
            if(type->GetName() == assetTypeName) {
                assetType = type;
                break;
            }
        }
    }

    if(assetType == nullptr) {
        LOGF(Error, LogAsset, "Failed loading asset \"%s\". Type \"%s\" is unknown.", Path.c_str(), assetTypeName.c_str());
        return nullptr;
    }

    // TODO: Asset Data
    Asset* asset = assetType->CreateInstance();
    asset->path = Path;
    asset->className = assetTypeName;
    asset->name = "";
    asset->Deserialize(assetArtifact);

    if(knownDescriptor) {
        knownDescriptor->AssetPtr.emplace(asset);
    } else {
        AssetList.push_back({assetType, Path, asset});
    }

    LOGF(Log, LogAsset, "Successfully loaded asset \"%s\" of Type \"%s\".", Path.c_str(), assetTypeName.c_str());

    return asset;
}

Asset* AssetLibrary::GetAsset(std::string Path)
{
    for(auto desc : AssetList) {
        if(desc.Path == Path) {
            return desc.AssetPtr.value_or(nullptr);
        }
    }
    return nullptr;
}

void AssetLibrary::CrawlAssetsTyped(AssetType type)
{
    IPlatform* platform = IPlatform::Get();

    IDirectory* directory = platform->OpenDirectory("./Content", DIRECTORY_FLAG_RECURSIVE);
    if(!directory->Exists()) {
        LOG(Warning, LogAsset, "Content directory does not exist. Are assets cooked?");
        return;
    }

    for(IDirectory* child : directory->GetChildren())
    {
        if(child->IsDirectory())
        {
            // Make path analysis
        } else {
            Asset* asset = new Asset();
        }
    }
}

void AssetLibrary::CrawlAssetsAll(std::string LocalDomain)
{
    // TODO
    CrawlAssetsTyped(AssetType());
}