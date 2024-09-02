#pragma once

#include "../Vulkan/vkloader.h"

#include <vector>

struct ShaderCreateInfo
{
    VkRect2D Viewport;
    VkRect2D Scissors;
    
    struct {
        VkBool32 SampleShadingEnabled = VK_FALSE;
    } Multisampling;

    struct {
        VkBool32 Enabled = VK_FALSE;
    } Blending;
};

struct ShaderDescriptorLayout
{
    std::vector<VkDescriptorSetLayoutBinding> Bindings;

    ShaderDescriptorLayout* GenerateBinding(uint32_t binding, VkDescriptorType type, 
        uint32_t count = 1, VkSampler* samplers = nullptr, VkShaderStageFlags stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS) 
    {
        VkDescriptorSetLayoutBinding bindingStruct;
        bindingStruct.binding = binding;
        bindingStruct.descriptorType = type;
        bindingStruct.descriptorCount = count;
        bindingStruct.stageFlags = stageFlags;
        bindingStruct.pImmutableSamplers = samplers;

        Bindings.push_back(bindingStruct);
        return this;
    }
};

class ShaderCache
{

};

class IShader
{
    static const ShaderCreateInfo DefaultShaderCreateInfo;
    static const ShaderDescriptorLayout DefaultShaderDescriptorLayout;

    VkShaderModule vertModule;
    VkShaderModule fragModule;

    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;
    VkDescriptorSetLayout descSetLayout;
public:
    IShader(VkRenderPass renderPass, std::vector<char> vertexData, std::vector<char> fragmentData, 
            const ShaderDescriptorLayout& descLayout = IShader::DefaultShaderDescriptorLayout, 
            const ShaderCreateInfo& createInfo = IShader::DefaultShaderCreateInfo);
    ~IShader();

    inline VkPipeline GetPipeline() const {return pipeline;}
    inline VkPipelineLayout GetPipelineLayout() const {return pipelineLayout;}
};