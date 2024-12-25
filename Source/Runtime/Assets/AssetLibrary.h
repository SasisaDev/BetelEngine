#pragma once

#include <map>
#include <unordered_set>
#include <vector>
#include <optional>
#include <string>

#include "Asset.h"
//TODO: #include "Files/AssetFile.h"
#include "Files/BundleFile.h"
#include <Platform/Platform.h>

struct AssetDescriptor
{
    AssetType* Type;
    // Local path. E.g. "Editor/Content/Some/Asset"
    std::string Path;
    std::optional<Asset*> AssetPtr;
    uint32_t Usages = 0;
};

// TODO: Add Numeric ID API
class AssetLibrary
{
    friend class AssetGarbageCollector;
protected:
    std::vector<BundleFile*> BundleFiles;
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
    
    // TODO: Rework crawling
    void CrawlAssetsTyped(AssetType type);
    void CrawlAssetsAll(std::string LocalDomain);

    void CrawlAssetsRecursive(IDirectory* directory);
};