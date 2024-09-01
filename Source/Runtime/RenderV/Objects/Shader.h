#pragma once

#include "../Vulkan/vkloader.h"

#include <vector>

class ShaderCache
{

};

class IShader
{
    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;
public:
    IShader(VkRenderPass renderPass, const char* vertexData, const char* fragmentData);
    ~IShader();

    inline VkPipeline GetPipeline() const {return pipeline;}
    inline VkPipelineLayout GetPipelineLayout() const {return pipelineLayout;}
};