#include "Utility.h"
#include <Log/Logger.h>

uint32_t IRenderUtility::framesInFlight = 2;
uint32_t IRenderUtility::currentFrameInFlight = 0;

RenderQueueFamilyIndices IRenderUtility::FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    RenderQueueFamilyIndices indices;
    
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    VkBool32 presentSupport = false;
    uint32_t i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT && !indices.graphicsFamily.has_value()) {
            indices.graphicsFamily = i;
        }

        if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT && !indices.computeFamily.has_value()) {
            indices.computeFamily = i;
        }

        if(surface != 0)
        {
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (presentSupport) {
                indices.presentFamily = i;
            }
        }

        if (indices.IsComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

uint32_t IRenderUtility::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(GetPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    LOG(Fatal, LogRender, "Could not find memory type for buffer allocation.");
    return 0;
}

VkDevice IRenderUtility::GetDevice()
{
    return vkloader::GetLogicalDevice();
}

VkPhysicalDevice IRenderUtility::GetPhysicalDevice()
{
    return vkloader::GetPhysicalDevice();
}