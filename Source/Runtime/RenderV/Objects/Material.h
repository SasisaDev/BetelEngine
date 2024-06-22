#pragma once

#include "Shader.h"

class IMaterial
{
protected:
    IShader* pShader;
public:

    void Bind(VkCommandBuffer cmd);
};