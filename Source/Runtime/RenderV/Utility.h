#pragma once

#include <vector>
#include <optional>

#include "Vulkan/vkloader.h"

struct RenderQueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    std::optional<uint32_t> computeFamily;

    bool IsComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value() && computeFamily.has_value();
    }
};

class IRenderUtility
{
    static uint32_t framesInFlight;
    static uint32_t currentFrameInFlight;
    static VkCommandPool singleTimePool;
public:
    static RenderQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface = 0);
    static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    static VkDevice GetDevice();
    static VkPhysicalDevice GetPhysicalDevice();
    static VkInstance GetInstance();

    static VkQueue GetDeviceQueue(uint32_t family = 0) {
        VkQueue queue;
        vkGetDeviceQueue(GetDevice(), family, 0, &queue);
        return queue;
    }

    static inline uint32_t GetFramesInFlight() {return framesInFlight;}
    static inline void SetFramesInFlight(uint32_t fif) {framesInFlight = fif;}
    
    static inline uint32_t GetCurrentFrameInFlight() {return currentFrameInFlight;}
    static inline void SetCurrentFrameInFlight(uint32_t curfif) {currentFrameInFlight = curfif;}

    static VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    static VkFormat FindDepthFormat();

    static inline bool HasStencilComponent(VkFormat format) {return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;}
     
    static inline VkCommandBuffer StartSingleTimeCommandBuffer() {
        if(singleTimePool == VK_NULL_HANDLE) {
            VkCommandPoolCreateInfo poolInfo = {};
            poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            poolInfo.queueFamilyIndex = FindQueueFamilies(GetPhysicalDevice()).graphicsFamily.value();

            vkCreateCommandPool(GetDevice(), &poolInfo, nullptr, &singleTimePool);
        }

        VkCommandBuffer cmdBuffer;

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandBufferCount = 1;
        allocInfo.commandPool = singleTimePool;
        vkAllocateCommandBuffers(GetDevice(), &allocInfo, &cmdBuffer);

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(cmdBuffer, &beginInfo);
        
        return cmdBuffer;
    }
    static inline void EndSingleTimeCommandBuffer(VkCommandBuffer cmdBuffer)
    {
        vkEndCommandBuffer(cmdBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cmdBuffer;

        VkQueue graphicsQueue = GetDeviceQueue(FindQueueFamilies(GetPhysicalDevice()).graphicsFamily.value());

        vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(graphicsQueue);
        
        vkFreeCommandBuffers(GetDevice(), singleTimePool, 1, &cmdBuffer);
    }

    static inline void Cleanup() {
        vkDestroyCommandPool(GetDevice(), singleTimePool, nullptr);
    }

    static inline void ImageBarrier(VkCommandBuffer cmdBuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout,
                                    VkPipelineStageFlags src = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VkPipelineStageFlags dst = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                    VkAccessFlags srcFlag = 0, VkAccessFlags dstFlag = 0)
    {
        VkImageMemoryBarrier imgMemBar = {};
        imgMemBar.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imgMemBar.oldLayout = oldLayout;
        imgMemBar.newLayout = newLayout;

        if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            imgMemBar.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

            // TODO Test format for stencil component
        } else {
            imgMemBar.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        }
        imgMemBar.image = image;
        imgMemBar.subresourceRange.baseMipLevel = 0;
        imgMemBar.subresourceRange.levelCount = 1;
        imgMemBar.subresourceRange.baseArrayLayer = 0;
        imgMemBar.subresourceRange.layerCount = 1;

        imgMemBar.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imgMemBar.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imgMemBar.srcAccessMask = srcFlag;
        imgMemBar.dstAccessMask = dstFlag;
    
        vkCmdPipelineBarrier(cmdBuffer, src, dst, 0, 0, nullptr, 0, nullptr, 1, &imgMemBar);
    }

    static inline void BeginDebugLabel(VkCommandBuffer cmdBuffer, const char* label, float r=0.5, float g=0.5, float b=0.5)
    {
#       if !NDEBUG
        VkDebugUtilsLabelEXT labelInfo = {
            .sType=VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pLabelName=label,
            .color = {r, g, b, 1}
        };
        vkCmdBeginDebugUtilsLabelEXT(cmdBuffer, &labelInfo);
#       endif
    }

    static inline void EndDebugLabel(VkCommandBuffer cmdBuffer)
    {
#       if !NDEBUG
        vkCmdEndDebugUtilsLabelEXT(cmdBuffer);
#       endif
    }
};