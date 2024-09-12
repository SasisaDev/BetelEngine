#pragma once

#include <memory>

#include <Assets/AssetSmartPointers.h>

#include "ShaderAsset.h"

class MaterialAsset : public Asset
{
protected:
    AssetPtr<ShaderAsset> Shader;
public:
};

class MaterialAssetType : public AssetType
{
    static bool bIsRegistered;
public:
    virtual Asset* CreateInstance() override {return new ShaderAsset;}
};