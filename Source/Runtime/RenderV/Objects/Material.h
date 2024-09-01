#pragma once

#include "Shader.h"

class IMaterial
{
protected:
    IShader* pShader;
public:
    IMaterial(IShader* shader) : pShader(shader) {}

    void Bind(VkCommandBuffer cmd);
};