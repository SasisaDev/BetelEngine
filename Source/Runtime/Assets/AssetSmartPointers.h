#pragma once

#include "AssetLibrary.h"

template <class AssetTypeT>
class AssetPtr
{
    AssetTypeT* assetPtr;
    std::string path;
public:
    AssetPtr() {}
    AssetPtr(AssetTypeT* ptr) : assetPtr(ptr) 
    {
        if(ptr != nullptr) {
            path = ptr->GetPath(); 
            AssetLibrary::Get().RegisterAssetUsage(path);
        }
    }

    AssetPtr(std::string defaultPath) : path(defaultPath) {
        AssetLibrary::Get().RegisterAssetUsage(path);
    }

    ~AssetPtr() 
    {
        if(assetPtr != nullptr) {
            AssetLibrary::Get().UnregisterAssetUsage(path);
        }
    }

    AssetTypeT* Load() 
    {
        if(assetPtr != nullptr) { 
            return assetPtr;
        }

        if(path.empty()) {
            return nullptr;
        }

        AssetLibrary& lib = AssetLibrary::Get();
        
        assetPtr = dynamic_cast<AssetTypeT*>(lib.LoadAsset(path));
        AssetLibrary::Get().RegisterAssetUsage(path);

        return assetPtr;
    }

    bool IsValid();
    bool IsLoaded();
};