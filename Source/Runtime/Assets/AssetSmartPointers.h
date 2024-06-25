#pragma once

#include "AssetLibrary.h"

template <class AssetType>
class AssetRef
{
    AssetType* assetPtr;
    std::string path;
public:
    AssetType* Load();

    bool IsValid();
    bool IsLoaded();
};