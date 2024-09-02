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
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSet;
public:
    IMaterial(IShader* shader, const ShaderDescriptorSet& descSetInfo);

    void Bind(VkCommandBuffer cmd);
};