#pragma once

#include <map>
#include <unordered_set>
#include <vector>
#include <optional>
#include <string>

#include "Asset.h"
#include <Platform/Platform.h>

struct AssetDescriptor
{
    AssetType* Type;
    // Local path. E.g. "Editor/Content/Some/Asset"
    std::string Path;
    std::optional<Asset*> AssetPtr;
    uint32_t Usages = 0;
};

class AssetLibrary
{
    friend class AssetGarbageCollector;
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

    bool RegisterAssetUsage(std::string path);
    bool UnregisterAssetUsage(std::string path);

    Asset* LoadAsset(std::string Path);
    Asset* GetAsset(std::string Path);
    void UnloadAsset(std::string Path, bool force = false);
    
    void CrawlAssetsTyped(AssetType type);
    void CrawlAssetsAll(std::string LocalDomain);

    void CrawlAssetsRecursive(IDirectory* directory);
};