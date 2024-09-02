#pragma once

#include "Shader.h"

struct ShaderDescriptorSet
{
    ShaderDescriptorLayout DescriptorLayout;
    
    
};

class IMaterial
{
protected:
    IShader* pShader;
public:
    IMaterial(IShader* shader) : pShader(shader) {}

    void Bind(VkCommandBuffer cmd);
};