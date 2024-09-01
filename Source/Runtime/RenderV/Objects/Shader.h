#pragma once

#include "../Vulkan/vkloader.h"

#include <vector>

class ShaderCache
{

};

class IShader
{
    VkShaderModule vertModule;
    VkShaderModule fragModule;

    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;
public:
    IShader(VkRenderPass renderPass, std::vector<char> vertexData, std::vector<char> fragmentData);
    ~IShader();

    inline VkPipeline GetPipeline() const {return pipeline;}
    inline VkPipelineLayout GetPipelineLayout() const {return pipelineLayout;}
};