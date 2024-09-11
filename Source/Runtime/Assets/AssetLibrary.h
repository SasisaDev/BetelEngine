#pragma once

#include <map>
#include <unordered_set>
#include <vector>
#include <optional>
#include <string>

#include "Asset.h"

struct AssetDescriptor
{
    AssetType* Type;
    std::string Path;
    std::optional<Asset*> AssetPtr;
};

class AssetLibrary
{
protected:
    std::vector<AssetType*> RegisteredAssetTypes;
    std::vector<AssetDescriptor> AssetList;
public:
    static AssetLibrary& Get();

    template <typename AssetTypeT>
    static bool RegisterAssetType() {
        Get().RegisteredAssetTypes.push_back(new AssetTypeT);
        return true;
    }

    Asset* LoadAsset(std::string Path);
    Asset* GetAsset(std::string Path);
    
    void CrawlAssetsTyped(AssetType type);
    void CrawlAssetsAll();
};