#pragma once

#include <memory>

#include <Assets/Asset.h>
#include <RenderV/Render.h>

class ShaderAsset : public Asset
{
protected:
    std::shared_ptr<IShader> pShader;
    
public:
    std::string ShaderLayerType = "WorldRenderLayer";
    
};