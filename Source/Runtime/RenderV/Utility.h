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
public:
    static RenderQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface = 0);
};