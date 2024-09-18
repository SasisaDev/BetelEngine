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
    std::string VertexShaderPath, FragmentShaderPath;
    bool SampleShadingEnable = false;
    bool BlendEnable = true;

    inline IShader* GetShader() const {return pShader.get();}
    
    virtual void Deserialize(Artifact& data) override;
};

class ShaderAssetType : public AssetType
{
    static bool bIsRegistered;
public:
    inline virtual std::string GetName() const override {return "Shader";}

    virtual Asset* CreateInstance() override {return new ShaderAsset;}
};