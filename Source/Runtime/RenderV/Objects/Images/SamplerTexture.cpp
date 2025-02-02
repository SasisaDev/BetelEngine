#include "SamplerTexture.h"
#include <Log/Logger.h>
#include <RenderV/Utility.h>

ISamplerTexture::~ISamplerTexture()
{
    VkDevice dev = IRenderUtility::GetDevice();

    vkDeviceWaitIdle(dev);

    vkDestroyImage(dev, image, nullptr);
    vkFreeMemory(dev, imageMemory, nullptr);
    vkDestroySampler(dev, sampler, nullptr);
    vkDestroyImageView(dev, imageView, nullptr);
    buffer.reset();
}

void ISamplerTexture::InitializeTexture(int dimX, int dimY, unsigned char* data, int channels)
{
    dimensionX = dimX;
    dimensionY = dimY;
    textureChannels = channels;

    VkDeviceSize imageSize = dimX * dimY * channels;

    buffer = std::make_unique<Buffer>(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, data);

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(dimX);
    imageInfo.extent.height = static_cast<uint32_t>(dimY);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    // TODO: Research
    switch(channels)
    {
        case 4: {
            imageInfo.format = imageFormat = VK_FORMAT_R8G8B8A8_SRGB;
            break;
        }
        case 3: {
            imageInfo.format = imageFormat = VK_FORMAT_R8G8B8_SRGB;
            break;
        }
        case 2: {
            imageInfo.format = imageFormat = VK_FORMAT_R8G8_SRGB;
            break;
        }
        case 1: {
            imageInfo.format = imageFormat = VK_FORMAT_R8_SRGB;
            break;
        }
        default: {
            LOG(Error, LogTexture, "Unsupported amount of texture channels");
            return;
        }
    }
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;

    if (vkCreateImage(IRenderUtility::GetDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS) 
    {
        LOG(Error, LogTexture, "Could not create VkImage object");
        return;
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(IRenderUtility::GetDevice(), image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = IRenderUtility::FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(IRenderUtility::GetDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) 
    {
        LOG(Error, LogTexture, "Could not allocate VkDeviceMemory for image");
        return;
    }

    vkBindImageMemory(IRenderUtility::GetDevice(), image, imageMemory, 0);

    UpdateTexture(data);

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = imageFormat;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(IRenderUtility::GetDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        LOG(Error, LogTexture, "Could not create VkImageView");
        return;
    }

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_NEAREST;
    samplerInfo.minFilter = VK_FILTER_NEAREST;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 0;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(IRenderUtility::GetDevice(), &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
        LOG(Error, LogTexture, "Could not create VkSampler");
        return;
    }
}

void ISamplerTexture::UpdateTexture(unsigned char* data)
{
    VkCommandBuffer cmdBuffer = IRenderUtility::StartSingleTimeCommandBuffer();
    IRenderUtility::BeginDebugLabel(cmdBuffer, "Texture Transfer", 1.0f, 60/256.f, 205/256.f);
    IRenderUtility::ImageBarrier(cmdBuffer, image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        static_cast<unsigned int>(dimensionX),
        static_cast<unsigned int>(dimensionY),
        1
    };

    vkCmdCopyBufferToImage(cmdBuffer, buffer->GetBufferObject(), image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    
    IRenderUtility::ImageBarrier(cmdBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    IRenderUtility::EndDebugLabel(cmdBuffer);
    IRenderUtility::EndSingleTimeCommandBuffer(cmdBuffer);
}