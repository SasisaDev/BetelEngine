#pragma once

#include "../Vulkan/vkloader.h"

#include <vector>

class IComputeShader
{
    VkShaderModule fragModule;

    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;
    VkDescriptorSetLayout descSetLayout;
public:
    IComputeShader(std::vector<char> data);
    ~IComputeShader();

    inline bool IsValid() const {return pipeline != VK_NULL_HANDLE;}

    inline VkPipeline GetPipeline() const {return pipeline;}
    inline VkPipelineLayout GetPipelineLayout() const {return pipelineLayout;}
};