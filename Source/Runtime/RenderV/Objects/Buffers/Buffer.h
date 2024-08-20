#pragma once

#include "../../Vulkan/vkloader.h"

#include <type_traits>

template<typename BufferDataType, size_t BufferDataSize = sizeof(BufferDataType)>
class Buffer
{
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
    BufferDataType* bufferData;

public:
    Buffer(VkDevice device, BufferDataType* initialBufferData = nullptr);

    void Write(VkDevice device, BufferDataType* newData);
    BufferDataType* Read(VkDevice device);
};

template<typename BufferDataType, size_t BufferDataSize>
Buffer<BufferDataType, BufferDataSize>::Buffer(VkDevice device, BufferDataType* initialBufferData)
{
    
}