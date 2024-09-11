#pragma once

#include "AssetLibrary.h"

template <class AssetTypeT>
class AssetPtr
{
    AssetTypeT* assetPtr;
    std::string path;
public:
    AssetPtr(std::string defaultPath);

    AssetTypeT* Load() 
    {
        if(assetPtr != nullptr) { 
            return assetPtr;
        }

        AssetLibrary& lib = AssetLibrary::Get();
        
        assetPtr = dynamic_cast<AssetTypeT*>(lib.GetAsset(path));
        if(assetPtr == nullptr) {
            assetPtr = lib.LoadAsset(path);
        }

        return assetPtr;
    }

    bool IsValid();
    bool IsLoaded();
};