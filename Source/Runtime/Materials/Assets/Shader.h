#pragma once

#include <memory>

#include <Object/Object.h>
#include <RenderV/Render.h>

class ObjShader : public Object
{
protected:
    std::shared_ptr<IShader> pShader;
    
public:
    std::string ShaderLayerType = "WorldRenderLayer";
    bool bIsComputeShader = false;
    std::string VertexShaderPath = {}, FragmentShaderPath = {};
    std::string ComputeShaderPath = {};
    bool SampleShadingEnable = false;
    bool BlendEnable = true;
    bool AlphaBlendEnable = true;

    inline IShader* GetShader() const {return pShader.get();}
};

class ObjShaderType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new ObjShader; }
    virtual bool CanSpawnIntoWorld() override {return false;}
    virtual std::string_view DisplayName() override {return "Shader";}
};