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
public:
    static RenderQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface = 0);
    static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    static VkDevice GetDevice();
    static VkPhysicalDevice GetPhysicalDevice();

    static inline uint32_t GetFramesInFlight() {return framesInFlight;}
    static inline void SetFramesInFlight(uint32_t fif) {framesInFlight = fif;}
    
    static inline uint32_t GetCurrentFrameInFlight() {return currentFrameInFlight;}
    static inline void SetCurrentFrameInFlight(uint32_t curfif) {currentFrameInFlight = curfif;}

    static inline void ImageBarrier(VkCommandBuffer cmdBuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout,
                                    VkPipelineStageFlags src = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VkPipelineStageFlags dst = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                    VkAccessFlags srcFlag = 0, VkAccessFlags dstFlag = 0)
    {
        VkImageMemoryBarrier imgMemBar = {};
        imgMemBar.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imgMemBar.oldLayout = oldLayout;
        imgMemBar.newLayout = newLayout;

        imgMemBar.image = image;
        imgMemBar.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
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