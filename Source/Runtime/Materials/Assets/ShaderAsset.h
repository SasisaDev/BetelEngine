#pragma once

#include <memory>

#include <Assets/AssetLibrary.h>
#include <RenderV/Render.h>

class ShaderAsset : public Asset
{
protected:
    std::shared_ptr<IShader> pShader;
    
public:
    std::string ShaderLayerType = "WorldRenderLayer";
    bool SampleShadingEnable = false;
    bool BlendEnable = true;
    
};

class ShaderAssetType : public AssetType
{
    static bool bIsRegistered;
public:
    inline virtual std::string GetName() const override {return "Shader";}

    virtual Asset* CreateInstance() override {return new ShaderAsset;}
};