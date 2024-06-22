#pragma once

#include <vector>

#include "Layer.h"

class IRenderCompositionInitializer
{

};

class RenderCompositionInitializerSurface : public IRenderCompositionInitializer
{

};

class RenderCompositionInitializerImage : public IRenderCompositionInitializer
{

};

enum ERenderCompositionType
{
    RENDER_COMPOSITION_TYPE_SURFACE = 0,
    RENDER_COMPOSITION_TYPE_IMAGE = 1
};

class RenderComposition
{
protected:
    VkSwapchainKHR swapchain;
    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
    std::vector<VkFramebuffer> framebuffers;
    VkSurfaceKHR surface;

    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    bool bIsPresentable = false;

    std::vector<RenderLayer*> Layers;
    RenderLayerDependency DependencyList;
public:

    bool Initialize(IRenderCompositionInitializer* initializer);
    
    bool Recreate(IRenderCompositionInitializer* initializer);

    uint32_t AddLayer(RenderLayer* layer);
    RenderLayer* GetLayer(uint32_t idx);

    void StartFrame();
    void Render(float DeltaTime);
    void EndFrame();

    bool Deinitialize();
};