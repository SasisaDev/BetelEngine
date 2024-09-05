#include "Composition.h"
#include <RenderV/Utility.h>
#include <Log/Logger.h>

void RenderCompositionInitializerSurface::Initialize(IRenderComposition* composition) 
{
    composition->surface = surface;

    composition->AddLayerRefs(layerRefs);

    VkPhysicalDevice physDevice = IRenderUtility::GetPhysicalDevice();
    VkDevice device = IRenderUtility::GetDevice();

    // Create Swapchain
    // TODO: swapchain support info
    composition->imageFormat = VK_FORMAT_B8G8R8A8_SRGB;

    std::vector<VkSurfaceFormatKHR> availableFormats;
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, nullptr);

    if (formatCount == 0) {
        LOG(Fatal, LogRender, "No surface formats available");
    }

    availableFormats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, availableFormats.data());

    VkSurfaceFormatKHR resultFormat;
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            resultFormat = availableFormat;
        }
    }

    VkSurfaceFormatKHR surfaceFormat = resultFormat;
    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

    composition->extent = extent;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.surface = surface;

    createInfo.minImageCount = 2;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    auto queueFamilyIndices = IRenderUtility::FindQueueFamilies(physDevice, surface);

    uint32_t indices[] = {queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value()};

    if (queueFamilyIndices.graphicsFamily != queueFamilyIndices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = indices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &(composition->swapchain)) != VK_SUCCESS) {
        LOG(Fatal, LogRender, "failed to create swap chain!");
    }

    // Fetch images
    uint32_t imageCount;
    vkGetSwapchainImagesKHR(device, composition->swapchain, &imageCount, nullptr);
    composition->images.resize(imageCount);
    vkGetSwapchainImagesKHR(device, composition->swapchain, &imageCount, composition->images.data());

    // Create image views
    composition->imageViews.resize(imageCount);

    for (size_t i = 0; i < composition->images.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = composition->images[i];

        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = composition->imageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device, &createInfo, nullptr, &composition->imageViews[i]) != VK_SUCCESS) {
            LOG(Fatal, LogRender, "failed to create image views.");
        }
    }

    if(composition->GetLayerRefs().size() <= 0) {
        return;
    }

    // Create framebuffers
    composition->framebuffers.resize(composition->imageViews.size());
    for (size_t i = 0; i < composition->imageViews.size(); i++) {
        VkImageView attachments[] = {
            composition->imageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        
        // TODO: Make more robust system
        VkRenderPass pass;
        for(IRenderLayerRef* ref : composition->GetLayerRefs()) {
            if (ref->GetParentLayer()->GetRenderPass() != VK_NULL_HANDLE) {
                pass = ref->GetParentLayer()->GetRenderPass();
                break;
            }
        }

        framebufferInfo.renderPass = pass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = composition->extent.width;
        framebufferInfo.height = composition->extent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &composition->framebuffers[i]) != VK_SUCCESS) {
            LOG(Fatal, LogRender,  "failed to create framebuffer!");
        }
    }

    VkSemaphoreCreateInfo semaphoreCreateInfo;
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.flags = 0;
    semaphoreCreateInfo.pNext = nullptr;

    vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &composition->aquireSemaphore);
}

void RenderCompositionInitializerImage::Initialize(IRenderComposition* composition) 
{
    
}

IRenderComposition::IRenderComposition()
    : Layers()
{

}

bool IRenderComposition::Initialize(IRenderCompositionInitializer* initializer)
{
    compositionType = initializer->GetType();

    initializer->Initialize(this);

    for(IRenderLayerRef* layerRef : Layers) {
        layerRef->Initialize(IRenderUtility::GetDevice(), DependencyList);
    }

    return true;
}

uint32_t IRenderComposition::AddLayerRef(IRenderLayerRef* ref)
{  
    Layers.push_back(ref);
    ref->SetParentComposition(this);
    return Layers.size() - 1;
}

std::pair<uint32_t, uint32_t> IRenderComposition::AddLayerRefs(std::vector<IRenderLayerRef*> &refs)
{  
    uint32_t firstID = Layers.size();
    for(IRenderLayerRef* ref : refs)
    {
        Layers.push_back(ref);
        ref->SetParentComposition(this);
    }
    return std::pair<uint32_t, uint32_t>(firstID, Layers.size() - 1);
}

void IRenderComposition::Render(VkCommandBuffer cmdBuffer)
{
    if(compositionType == ERenderCompositionType::RENDER_COMPOSITION_TYPE_SURFACE)
    {
        vkAcquireNextImageKHR(IRenderUtility::GetDevice(), swapchain, UINT64_MAX, aquireSemaphore, VK_NULL_HANDLE, &targetImageId);
    }

    IRenderLayerRef* previousLayer = nullptr;

    // TODO: Fix this mess

    //vkCmdPipelineBarrier(cmdbuffer);

    for(size_t layerRefId = 0; layerRefId < Layers.size(); layerRefId++)
    {
        Layers[layerRefId]->GetParentLayer()->Prepare(cmdBuffer, Layers[layerRefId], previousLayer);

        previousLayer = Layers[layerRefId];
    }

    previousLayer = nullptr;
    for(size_t layerRefId = 0; layerRefId < Layers.size(); layerRefId++)
    {
        Layers[layerRefId]->GetParentLayer()->Render(cmdBuffer, Layers[layerRefId], previousLayer);

        previousLayer = Layers[layerRefId];
    }

    targetImageId = (targetImageId + 1) % GetFramesInFlight();
}