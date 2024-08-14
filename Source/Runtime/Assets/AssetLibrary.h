#pragma once

#include <unordered_map>
#include <unordered_multimap>
#include <unordered_multiset>
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
    std::unordered_multiset<AssetDescriptor> AssetList;
public:
    AssetLibrary& Get();

    void RegisterAssetType(AssetType type);
    
    void CrawlAssetsTyped(AssetType type);
    void CrawlAssetsAll();
};