#pragma once

#include <vector>

#include "LayerRef.h"
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

class IRenderComposition
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

    std::vector<IRenderLayerRef*> Layers;
    RenderLayerDependency DependencyList;
public:

    bool Initialize(IRenderCompositionInitializer* initializer);
    
    bool Recreate(IRenderCompositionInitializer* initializer);

    template <typename LayerType>
    uint32_t AddLayerRef();
    IRenderLayerRef* GetLayer(uint32_t idx);

    void StartFrame();
    void Render(float DeltaTime);
    void EndFrame();

    bool Deinitialize();
};

template <typename LayerType>
inline uint32_t IRenderComposition::AddLayerRef()
{
    IRenderLayerRef* layerRef = LayerType::CreateRef();
    
    Layers.push_back(layerRef);
}