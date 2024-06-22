#pragma once

#include "../Vulkan/vkloader.h"

#include <vector>

class ShaderCache
{

};

class IShader
{
    VkPipeline pipeline;
public:
    IShader(VkDevice device, VkRenderPass renderPass, std::vector<unsigned char> vertexData, std::vector<unsigned char> fragmentData);
};