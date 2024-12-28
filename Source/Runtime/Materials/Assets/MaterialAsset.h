#pragma once

#include <memory>

#include <Assets/AssetSmartPointers.h>
#include <RenderV/Objects/Material.h>

#include "ShaderAsset.h"

class MaterialAsset : public Asset
{
protected:
    AssetPtr<ShaderAsset> Shader;
    std::shared_ptr<IMaterial> pMaterial;
public:
};

class MaterialAssetType : public AssetType
{
    static bool bIsRegistered;
public:
    inline virtual std::string GetName() const override {return "Material";}

    virtual Asset* CreateInstance() override {return new ShaderAsset;}
};