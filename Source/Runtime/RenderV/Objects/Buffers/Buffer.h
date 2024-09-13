#pragma once

#include "../../Vulkan/vkloader.h"
#include <RenderV/Utility.h>
#include <Log/Logger.h>

#include <type_traits>

class Buffer
{
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
    void* bufferData;
    size_t bufferSize;

public:
    Buffer(size_t BufferDataSize, VkBufferUsageFlags usage = VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM, void* initialBufferData = nullptr);
    ~Buffer();

    inline VkBuffer GetBufferObject() const { return buffer; }
    inline VkDeviceMemory GetDeviceMemoryObject() const { return bufferMemory; }
    inline size_t GetSize() const {return bufferSize;}

    void Write(void* newData);
    void* Read();
};