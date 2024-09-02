#pragma once

#include "AssetLibrary.h"

template <class AssetType>
class AssetPtr
{
    AssetType* assetPtr;
    std::string path;
public:
    AssetPtr(std::string defaultPath);

    AssetType* Load() 
    {
        if(assetPtr != nullptr) { 
            return assetPtr;
        }

        AssetLibrary& lib = AssetLibrary::Get();
        
        assetPtr = dynamic_cast<AssetType*>(lib.GetAsset(path));
        if(assetPtr == nullptr) {
            assetPtr = lib.LoadAsset(path);
        }

        return assetPtr;
    }

    bool IsValid();
    bool IsLoaded();
};