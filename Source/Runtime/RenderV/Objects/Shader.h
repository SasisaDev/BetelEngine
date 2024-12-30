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
        VkBool32 WriteAlpha = VK_FALSE;
    } Blending;
    
    struct {
        VkBool32 Enabled = VK_TRUE;
    } Depth;

    std::vector<VkVertexInputAttributeDescription> VertexInputAtrributeDescriptions;
    std::vector<VkVertexInputBindingDescription> VertexInputBindingDescriptions;
};

struct ShaderDescriptorLayout
{
    std::vector<VkDescriptorSetLayoutBinding> Bindings;
    std::vector<VkPushConstantRange> PushConstantRanges;

    ShaderDescriptorLayout* SetPushConstantRange(uint32_t size, uint32_t offset = 0, VkShaderStageFlags shaderStage = VK_SHADER_STAGE_VERTEX_BIT) {
        PushConstantRanges.push_back({});
        const size_t Idx = PushConstantRanges.size()-1;
	    // Push constant range starts at the beginning
	    PushConstantRanges[Idx].offset = offset;
	    // Push constant range takes up the size of a MeshPushConstants struct
	    PushConstantRanges[Idx].size = size;
	    // Push constant range is accessible only in the vertex shader
	    PushConstantRanges[Idx].stageFlags = shaderStage;

        return this;
    }

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

    ShaderDescriptorLayout descriptorLayoutParameters;
public:
    IShader(VkRenderPass renderPass, std::vector<char> vertexData, std::vector<char> fragmentData, 
            const ShaderDescriptorLayout& descLayout = IShader::DefaultShaderDescriptorLayout, 
            const ShaderCreateInfo& createInfo = IShader::DefaultShaderCreateInfo);
    ~IShader();

    inline bool IsValid() const {return pipeline != VK_NULL_HANDLE;}

    inline VkPipeline GetPipeline() const {return pipeline;}
    inline VkPipelineLayout GetPipelineLayout() const {return pipelineLayout;}
    inline VkDescriptorSetLayout GetDescriptorSetLayout() const {return descSetLayout;}

    inline const ShaderDescriptorLayout& GetShaderDescriptorLayoutStructure() const {return descriptorLayoutParameters;}
};