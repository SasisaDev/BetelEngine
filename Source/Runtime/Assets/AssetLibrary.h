#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <optional>
#include <string>

#include "Asset.h"
#include "AssetTypes.h"

struct AssetDescriptor
{
    AssetType* Type;
    std::string Path;
    std::optional<Asset*> Asset;
};

class AssetLibrary
{
protected:
    std::vector<AssetType> RegisteredAssetTypes;
    std::vector<AssetDescriptor> AssetList;
public:
    static AssetLibrary& Get();

    Asset* LoadAsset(std::string Path);
    Asset* GetAsset(std::string Path);

    void RegisterAssetType(AssetType type);
    
    void CrawlAssetsTyped(AssetType type);
    void CrawlAssetsAll();
};