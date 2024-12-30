#pragma once

#include "Shader.h"

class IMaterial
{
protected:
    IShader* pShader;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
public:
    IMaterial(IShader* shader);
    ~IMaterial();

    void SetBuffer(uint32_t binding, VkBuffer buffer, uint32_t size, bool StorageBuffer = false);
    void SetSampler(uint32_t binding, VkImageView view, VkSampler sampler);
    void SetSamplerRenderTarget(uint32_t binding, std::vector<VkImageView> views, VkSampler sampler);

    inline VkDescriptorSet Get(uint32_t id) const {return descriptorSets[id];}
    inline IShader* GetShader() const {return pShader;}

    void Bind(VkCommandBuffer cmd);
};