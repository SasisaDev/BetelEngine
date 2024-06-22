#pragma once

#include <unordered_map>
#include <vector>

#include "Asset.h"
#include "AssetTypes.h"

class AssetLibrary
{
protected:
    std::vector<AssetType> AssetTypes;
    std::unordered_map<AssetType, Asset> AssetMap;
public:
    AssetLibrary& Get();

    void RegisterAssetType(AssetType type);
    
    void CrawlAssetsTyped(AssetType type);
    void CrawlAssetsAll();
};