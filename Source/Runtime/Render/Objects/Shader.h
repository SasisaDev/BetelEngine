#pragma once

#include "../Vulkan/vkloader.h"

#include <vector>

class Shader
{
    VkPipeline pipeline;
public:
    Shader(VkRenderPass renderPass, std::vector<unsigned char> vertexData, std::vector<unsigned char> fragmentData);
};