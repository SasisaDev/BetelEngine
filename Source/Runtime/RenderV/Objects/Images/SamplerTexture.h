#pragma once

#include <RenderV/Vulkan/vkloader.h>
#include "../Buffers/Buffer.h"

class ISamplerTexture
{
protected:
    Buffer *buffer;
    VkDeviceMemory imageMemory;
    VkImage image;
    VkImageView imageView;
    VkSampler sampler;

    int dimensionX = 0, dimensionY = 0, textureChannels = 0;
    VkFormat imageFormat;
public:
    ISamplerTexture(){}
    ISamplerTexture(int dimX, int dimY, unsigned char* data = nullptr, int channels = 4){InitializeTexture(dimX, dimY, data, channels);}
    ~ISamplerTexture();
    

    inline Buffer* GetBuffer() {return buffer;}
    inline VkDeviceMemory GetImageMemory() {return imageMemory;}
    inline VkImage GetImage() {return image;}
    inline VkImageView GetImageView() {return imageView;}
    inline VkSampler GetSampler() {return sampler;}

    void InitializeTexture(int dimX, int dimY, unsigned char* data = nullptr, int channels = 4);
    void UpdateTexture(unsigned char* data);
};