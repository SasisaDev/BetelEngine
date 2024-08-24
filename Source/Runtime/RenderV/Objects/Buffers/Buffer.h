#pragma once

#include "../../Vulkan/vkloader.h"
#include <RenderV/Utility.h>
#include <Log/Logger.h>

#include <type_traits>

template<typename BufferDataType, size_t BufferDataSize = sizeof(BufferDataType)>
class Buffer
{
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
    void* bufferData;

public:
    Buffer(VkBufferUsageFlags usage, BufferDataType* initialBufferData = nullptr);
    ~Buffer();

    inline VkBuffer GetBufferObject() const { return buffer; }
    inline VkDeviceMemory GetDeviceMemoryObject() const { return bufferMemory; }

    void Write(BufferDataType* newData);
    BufferDataType* Read();
};

template<typename BufferDataType, size_t BufferDataSize>
Buffer<BufferDataType, BufferDataSize>::Buffer(VkBufferUsageFlags usage, BufferDataType* initialBufferData)
{
    VkDevice device = IRenderUtility::GetDevice();

    VkBufferCreateInfo buffCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = BufferDataSize,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    };  

    if (vkCreateBuffer(device, &buffCreateInfo, nullptr, &buffer) != VK_SUCCESS) 
    {
        LOG(Fatal, LogRender, "Buffer creation failed");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = IRenderUtility::FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        LOG(Fatal, LogRender, "Buffer memory allocation failed");
    }

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

template<typename BufferDataType, size_t BufferDataSize>
Buffer<BufferDataType, BufferDataSize>::~Buffer()
{
    VkDevice device = IRenderUtility::GetDevice();

    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, bufferMemory, nullptr);
}