#include "WorldRenderLayer.h"
#include <Engine/EngineDelegates.h>
#include <functional>
#include <Log/Logger.h>
#include <RenderV/Utility.h>

#include <RenderV/Composition.h>

WorldRenderLayerRef::WorldRenderLayerRef()
{
    std::function<void(World*)> func;
    func = std::bind(&WorldRenderLayerRef::onWorldLoad, this, std::placeholders::_1);
    EngineDelegates::OnWorldLoad.AddBind(func);


}

bool WorldRenderLayerRef::Initialize(VkDevice device, RenderDependencyList<IRenderLayerRef>& DependencyList)
{
    VkPhysicalDevice physDevice = IRenderUtility::GetPhysicalDevice();

    const size_t swapchainImageCount = 2;

    // Create Swapchain
    // TODO: swapchain support info
    imageFormat = VK_FORMAT_B8G8R8A8_SRGB;

    pixelPerfectImages.resize(swapchainImageCount);
    pixelPerfectImageViews.resize(swapchainImageCount);
    pixelPerfectImageFramebuffers.resize(swapchainImageCount);
    pixelPerfectImageMemories.resize(swapchainImageCount);

    for(size_t i = 0; i < swapchainImageCount; i++) {
        VkImageCreateInfo imgInfo;
        imgInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imgInfo.flags = 0;
        imgInfo.pNext = nullptr;
        imgInfo.imageType = VK_IMAGE_TYPE_2D;
		imgInfo.format = imageFormat;
		imgInfo.extent.width = viewport.width;
		imgInfo.extent.height = viewport.height;
		imgInfo.extent.depth = 1;
		imgInfo.mipLevels = 1;
		imgInfo.arrayLayers = 1;
		imgInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imgInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imgInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        auto indices = IRenderUtility::FindQueueFamilies(IRenderUtility::GetPhysicalDevice());
        imgInfo.queueFamilyIndexCount = 1;
        imgInfo.pQueueFamilyIndices = &indices.graphicsFamily.value();
        imgInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imgInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(device, &imgInfo, nullptr, &pixelPerfectImages[i]) != VK_SUCCESS) {
            LOG(Fatal, LogWorldRenderLayer, "failed to create image!");
        }

        VkMemoryAllocateInfo memAlloc;
        memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memAlloc.pNext = nullptr;
        //memAlloc.allocationSize = ;
        VkMemoryRequirements memReqs;

        vkGetImageMemoryRequirements(device, pixelPerfectImages[i], &memReqs);
		memAlloc.allocationSize = memReqs.size;
		memAlloc.memoryTypeIndex = IRenderUtility::FindMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if(vkAllocateMemory(device, &memAlloc, nullptr, &pixelPerfectImageMemories[i]) != VK_SUCCESS) {
            LOG(Fatal, LogWorldRenderLayer, "failed to allocate device memory.");
        }

		if(vkBindImageMemory(device, pixelPerfectImages[i], pixelPerfectImageMemories[i], 0) != VK_SUCCESS) {
            LOG(Fatal, LogWorldRenderLayer, "failed to bind memory.");
        }

        VkImageViewCreateInfo viewInfo;
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.flags = 0;
        viewInfo.pNext = nullptr;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = imageFormat;
		viewInfo.subresourceRange = {};
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		viewInfo.image = pixelPerfectImages[i];

        viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        if (vkCreateImageView(device, &viewInfo, nullptr, &pixelPerfectImageViews[i]) != VK_SUCCESS) {
            LOG(Fatal, LogWorldRenderLayer, "failed to create image views.");
        }

        VkFramebufferCreateInfo framebufferInfo;
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.flags = 0;
        framebufferInfo.pNext = nullptr;
        framebufferInfo.width = viewport.width;
        framebufferInfo.height = viewport.height;
        framebufferInfo.renderPass = GetParentLayer()->GetRenderPass();
        framebufferInfo.layers = 1;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = &pixelPerfectImageViews[i];
       
        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, pixelPerfectImageFramebuffers.data()) != VK_SUCCESS) {
            LOG(Fatal, LogWorldRenderLayer,  "failed to create framebuffer!");
        }
    }

    return true;
}

WorldRenderLayerRef* WorldRenderLayerRef::SetWorld(World* newWorld)
{
    world = newWorld;
    return this;
}

bool WorldRenderLayer::Initialize(VkDevice device)
{
    // Create Render Pass
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = VK_FORMAT_B8G8R8A8_SRGB;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        LOG(Fatal, LogRender, "failed to create render pass!");
    }

    return true;
}

void WorldRenderLayer::Prepare(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer)
{

}

void WorldRenderLayer::Render(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer)
{
    VkRenderPassBeginInfo passInfo;
    passInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    passInfo.pNext = nullptr;
    passInfo.renderPass = renderPass;
    VkClearValue clearVal = {{{0, 1, 0, 1}}};
    passInfo.pClearValues = &clearVal;
    passInfo.clearValueCount = 1;
    passInfo.framebuffer = ((WorldRenderLayerRef*)layerRef)->pixelPerfectImageFramebuffers[0]/*layerRef->GetParentComposition()->GetCurrentFramebuffer()*/;
    passInfo.renderArea.offset = {0, 0};
    passInfo.renderArea.extent = ((WorldRenderLayerRef*)layerRef)->viewport;

    vkCmdBeginRenderPass(cmdBuffer, &passInfo, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

    // Render
    VkViewport viewport;
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = ((WorldRenderLayerRef*)layerRef)->viewport.width;
    viewport.height = ((WorldRenderLayerRef*)layerRef)->viewport.height;
    //viewport.width = layerRef->GetParentComposition()->GetExtent().width;
    //viewport.height = layerRef->GetParentComposition()->GetExtent().height;

    vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);



    // Render End

    vkCmdEndRenderPass(cmdBuffer);
}

bool WorldRenderLayer::Deinitialize()
{
    vkDestroyRenderPass(IRenderUtility::GetDevice(), renderPass, nullptr);

    return true;
}