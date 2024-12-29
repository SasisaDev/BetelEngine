#pragma once

#include <RenderV/Vulkan/vkloader.h>

class Image
{
protected:
    VkImage image;
    VkDeviceMemory deviceMemory;
public:

    inline VkImage GetImageObject() {return image;}
    inline VkDeviceMemory GetDeviceMemoryObject() const { return deviceMemory; }
};