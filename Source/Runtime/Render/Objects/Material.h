#pragma once

#include "Shader.h"

class Material
{
protected:
    Shader* pShader;
public:

    void Bind(VkCommandBuffer cmd);
};