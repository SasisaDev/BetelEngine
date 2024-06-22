#pragma once

#include "Engine.h"
#include "Objects/Material.h"

#include <memory>

class IRenderFactory
{
public:
    static IRenderFactory& Get();

    IRenderEngine CreateEngine();

    std::unique_ptr<IShader> CreateShader(IRenderEngine* engine, IRenderLayer* layer, std::vector<unsigned char> vertCode, std::vector<unsigned char> fragCode);
    std::unique_ptr<IMaterial> CreateMaterial(IRenderEngine* engine, IShader* shader);
};