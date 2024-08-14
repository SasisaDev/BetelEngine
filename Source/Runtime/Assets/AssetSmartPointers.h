#pragma once

#include "AssetLibrary.h"

template <class AssetType>
class AssetPtr
{
    AssetType* assetPtr;
    std::string path;
public:
    AssetPtr(std::string defaultPath);

    AssetType* Load();

    bool IsValid();
    bool IsLoaded();
};