#pragma once

#include <memory>

#include "ShaderAsset.h"

class MaterialAsset : public Asset
{
protected:
    std::shared_ptr<IShader> pShader;
public:
};