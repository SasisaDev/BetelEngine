#pragma once

#include <memory>

#include <Assets/Asset.h>
#include <RenderV/Render.h>

class ShaderAsset : public Asset
{
protected:
    std::unique_ptr<IShader> pShader;
public:
};