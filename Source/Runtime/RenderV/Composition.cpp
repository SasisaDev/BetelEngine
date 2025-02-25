#include "Composition.h"
#include <RenderV/Utility.h>
#include <Log/Logger.h>
#include <array>

void RenderCompositionInitializerSurface::Initialize(IRenderComposition* composition) 
{
    if(extent.width <= 0 || extent.height <= 0) {
        if(composition->extent.width <= 0 || composition->extent.height <= 0) {
            LOG(Fatal, LogRender, "Could not initialize surface composition: Viewport Extent is zero!");
            return;
        } else {
            extent = composition->extent;
        }
    } else {
        composition->extent = extent;
    }

    if(surface != VK_NULL_HANDLE) {
        composition->surface = surface;
    } else {
        if(composition->surface == VK_NULL_HANDLE) {
            LOG(Fatal, LogRender, "Could not initialize surface composition: Surface is nullptr!");
            return;
        } else {
            surface = composition->surface;
        }
    }

    composition->GameViewport = {composition->offset, composition->extent};

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

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.surface = surface;

    createInfo.minImageCount = composition->GetFramesInFlight();
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    auto queueFamilyIndices = IRenderUtility::FindQueueFamilies(physDevice, surface);

    std::array<uint32_t, 2> indices = {queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value()};

    if (queueFamilyIndices.graphicsFamily != queueFamilyIndices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = indices.data();
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = composition->swapchain;

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
        // At the current time initial composition framebuffer creation
        // Requires a render pass, which we get from one of the existing layers,
        // Hoping for it to exist and be valid. If it's not, the crush will occur.
        VkRenderPass pass = {};
        for(IRenderLayerRef* ref : composition->GetLayerRefs()) {
            if (ref->GetParentLayer()->GetCompositionCompatRenderPass() != VK_NULL_HANDLE) {
                pass = ref->GetParentLayer()->GetCompositionCompatRenderPass();
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

    composition->aquireSemaphores.resize(composition->GetFramesInFlight());
    for(int i = 0; i < composition->GetFramesInFlight(); ++i) {
        vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &composition->aquireSemaphores[i]);
    }

    LOGF(Log, LogRender, "Successfully initialized composition for surface: %u", surface);
}

void RenderCompositionInitializerImage::Initialize(IRenderComposition* composition) 
{
    
}

IRenderComposition::IRenderComposition()
    : Layers()
{

}

bool IRenderComposition::Deinitialize()
{
    VkDevice dev = IRenderUtility::GetDevice();

    for(IRenderLayerRef* ref : Layers) {
        ref->Deinitialize(dev);
        delete ref;
    }

    for(VkSemaphore semaphore : aquireSemaphores) {
        vkDestroySemaphore(dev, semaphore, nullptr);
    }

    for(VkFramebuffer fb : framebuffers) {
        vkDestroyFramebuffer(dev, fb, nullptr);
    }

    for(VkImageView imgv : imageViews) {
        vkDestroyImageView(dev, imgv, nullptr);
    }

    vkDestroySwapchainKHR(dev, swapchain, nullptr);

    vkDestroySurfaceKHR(IRenderUtility::GetInstance(), surface, nullptr);

    return true;
}

IRenderComposition::~IRenderComposition()
{
    
}

bool IRenderComposition::Initialize(IRenderCompositionInitializer* initializer)
{
    compositionType = initializer->GetType();

    initializer->Initialize(this);

    LOG(Log, LogRender, "Composition Layer List: ");
    size_t layerI = 1;
    for(IRenderLayerRef* layerRef : Layers) {
        LOGF(Log, LogRender, "\t%u: %s", layerI++, layerRef->GetParentLayer()->GetName().c_str());
        layerRef->Initialize(IRenderUtility::GetDevice(), DependencyList);
    }

    // TODO: Dependency Injection Sorting

    return true;
}

bool IRenderComposition::Recreate(IRenderCompositionInitializer* initializer)
{
    vkDeviceWaitIdle(IRenderUtility::GetDevice());

    // Cleanup Swapchain
    if(compositionType == ERenderCompositionType::RENDER_COMPOSITION_TYPE_SURFACE) {
        for (size_t i = 0; i < framebuffers.size(); i++) {
            vkDestroyFramebuffer(IRenderUtility::GetDevice(), framebuffers[i], nullptr);
        }

        for (size_t i = 0; i < imageViews.size(); i++) {
            vkDestroyImageView(IRenderUtility::GetDevice(), imageViews[i], nullptr);
        }

        for (size_t i = 0; i < aquireSemaphores.size(); i++) {
            vkDestroySemaphore(IRenderUtility::GetDevice(), aquireSemaphores[i], nullptr);
        }
    }

    VkSwapchainKHR oldSwapchain = swapchain;

    initializer->Initialize(this);

    vkDestroySwapchainKHR(IRenderUtility::GetDevice(), oldSwapchain, nullptr);

    for(IRenderLayerRef* layerRef : Layers) {
        layerRef->Recreate();
    }

    bPauseRender = false;

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
    return std::pair<uint32_t, uint32_t>(firstID, static_cast<uint32_t>(Layers.size() - 1));
}

void IRenderComposition::NotifyLayersRecreateResources() 
{
    for(IRenderLayerRef* layer : Layers) {
        layer->Recreate();
    }
}

void IRenderComposition::MakeLayersDirty()
{
    for(IRenderLayerRef* layer : Layers) {
        layer->SetDirty();
    }
}

void IRenderComposition::StartFrame(VkCommandBuffer cmdBuffer)
{
    if(bCompositionDirty) {
        // TODO: Recreate();
        bCompositionDirty = false;
    } else {
        for(size_t layerRefId = 0; layerRefId < Layers.size(); layerRefId++)
        {
            if(Layers[layerRefId]->IsDirty())
            {
                Layers[layerRefId]->Recreate();
            }
        }
    }
}

void IRenderComposition::Render(VkCommandBuffer cmdBuffer)
{
    if(bPauseRender) {
        return;
    }

    if(compositionType == ERenderCompositionType::RENDER_COMPOSITION_TYPE_SURFACE)
    {
        vkAcquireNextImageKHR(IRenderUtility::GetDevice(), swapchain, 1, GetAquireSemaphore(), VK_NULL_HANDLE, &targetImageId);
    }

    IRenderLayerRef* previousLayer = nullptr;

    // Get image ready for render pass chain

#   ifdef EDITOR

    IRenderUtility::ImageBarrier(cmdBuffer, GetCurrentImage(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    VkClearColorValue clearValue {{0.015f, 0.015f, 0.015f, 1.0f}};
    VkImageSubresourceRange clearRange{ .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                        .baseMipLevel = 0,
                                        .levelCount = 1,
                                        .baseArrayLayer = 0,
                                        .layerCount = 1};
    vkCmdClearColorImage(cmdBuffer, GetCurrentImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearValue, 1, &clearRange);

    IRenderUtility::ImageBarrier(cmdBuffer, GetCurrentImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
#   else
    IRenderUtility::ImageBarrier(cmdBuffer, GetCurrentImage(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
#   endif

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

    //targetImageId = (targetImageId + 1) % GetFramesInFlight();
}