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