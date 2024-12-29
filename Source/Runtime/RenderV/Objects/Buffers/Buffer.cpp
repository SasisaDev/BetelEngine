#include "Buffer.h"

#include <cstring>

Buffer::Buffer(size_t BufferDataSize, VkBufferUsageFlags usage, void* initialBufferData)
    : bufferSize(BufferDataSize)
{
    bufferData = malloc(BufferDataSize);

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

    if(initialBufferData != nullptr) {
        vkMapMemory(IRenderUtility::GetDevice(), bufferMemory, 0, bufferSize, 0, &bufferData);
        std::memcpy(bufferData, initialBufferData, bufferSize);
        vkUnmapMemory(IRenderUtility::GetDevice(), bufferMemory);
    }
}

Buffer::~Buffer()
{
    VkDevice device = IRenderUtility::GetDevice();

    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, bufferMemory, nullptr);
}

void Buffer::Write(void* newData) 
{
    vkMapMemory(IRenderUtility::GetDevice(), bufferMemory, 0, bufferSize, 0, &bufferData);
    std::memcpy(bufferData, newData, bufferSize);
    vkUnmapMemory(IRenderUtility::GetDevice(), bufferMemory);
}

void* Buffer::Read()
{
    void* data = malloc(bufferSize); 
    vkMapMemory(IRenderUtility::GetDevice(), bufferMemory, 0, bufferSize, 0, &bufferData);
    std::memcpy(data, bufferData, bufferSize);
    vkUnmapMemory(IRenderUtility::GetDevice(), bufferMemory);
    return data;
}