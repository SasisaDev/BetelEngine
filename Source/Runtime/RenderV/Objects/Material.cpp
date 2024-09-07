#include "Material.h"
#include <Log/Logger.h>
#include <RenderV/Utility.h>

IMaterial::IMaterial(IShader* shader)
    : pShader(shader) 
{
    std::vector<VkDescriptorPoolSize> poolSizes;
    for(auto binding : shader->GetShaderDescriptorLayoutStructure().Bindings) {
        poolSizes.push_back({binding.descriptorType, binding.descriptorCount * IRenderUtility::GetFramesInFlight()});
    }

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = IRenderUtility::GetFramesInFlight();

    if (vkCreateDescriptorPool(IRenderUtility::GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        LOG(Fatal, LogRender, "failed to create descriptor pool!");
    }

    std::vector<VkDescriptorSetLayout> layouts(IRenderUtility::GetFramesInFlight(), shader->GetDescriptorSetLayout());

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = IRenderUtility::GetFramesInFlight();
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(IRenderUtility::GetFramesInFlight());
    if (vkAllocateDescriptorSets(IRenderUtility::GetDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        LOG(Fatal, LogRender, "failed to allocate descriptor sets!");
    }
}

void IMaterial::SetSampler(uint32_t binding, VkImageView view, VkSampler sampler)
{  
    for(VkDescriptorSet set : descriptorSets)
    {
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = view;
        imageInfo.sampler = sampler;

        VkWriteDescriptorSet descriptorWrites;

        descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites.pNext = nullptr;
        descriptorWrites.dstSet = set;
        descriptorWrites.dstBinding = binding;
        descriptorWrites.dstArrayElement = 0;
        descriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites.descriptorCount = 1;
        descriptorWrites.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(IRenderUtility::GetDevice(), 1, &descriptorWrites, 0, nullptr);
    }
}

void IMaterial::SetSamplerRenderTarget(uint32_t binding, std::vector<VkImageView> views, VkSampler sampler)
{
    for(int i = 0; i < descriptorSets.size(); ++i)
    {
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = views[i];
        imageInfo.sampler = sampler;

        VkWriteDescriptorSet descriptorWrites;

        descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites.pNext = nullptr;
        descriptorWrites.dstSet = descriptorSets[i];
        descriptorWrites.dstBinding = binding;
        descriptorWrites.dstArrayElement = 0;
        descriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites.descriptorCount = 1;
        descriptorWrites.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(IRenderUtility::GetDevice(), 1, &descriptorWrites, 0, nullptr);
    }
}

void IMaterial::SetBuffer(uint32_t binding, VkBuffer buffer, uint32_t size, bool StorageBuffer)
{
        for(VkDescriptorSet set : descriptorSets)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = size;

        VkWriteDescriptorSet descriptorWrites;

        descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites.pNext = nullptr;
        descriptorWrites.dstSet = set;
        descriptorWrites.dstBinding = binding;
        descriptorWrites.dstArrayElement = 0;
        descriptorWrites.descriptorType = ((StorageBuffer) ? VK_DESCRIPTOR_TYPE_STORAGE_BUFFER : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
        descriptorWrites.descriptorCount = 1;
        descriptorWrites.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(IRenderUtility::GetDevice(), 1, &descriptorWrites, 0, nullptr);
    }
}

IMaterial::~IMaterial() {
    vkDestroyDescriptorPool(IRenderUtility::GetDevice(), descriptorPool, nullptr);
}