#pragma once

#include <vector>

#include "LayerRef.h"
#include "Layer.h"

enum class ERenderCompositionType : uint8_t
{
    RENDER_COMPOSITION_TYPE_NONE = 0,
    RENDER_COMPOSITION_TYPE_SURFACE = 1,
    RENDER_COMPOSITION_TYPE_IMAGE = 2
};

struct IRenderCompositionInitializer
{
    virtual ERenderCompositionType GetType() const {return ERenderCompositionType::RENDER_COMPOSITION_TYPE_NONE;}

};

struct RenderCompositionInitializerSurface : public IRenderCompositionInitializer
{
    VkSurfaceKHR surface;
    
    // Functions

    virtual ERenderCompositionType GetType() const override {return ERenderCompositionType::RENDER_COMPOSITION_TYPE_SURFACE;}

};

struct RenderCompositionInitializerImage : public IRenderCompositionInitializer
{
    virtual ERenderCompositionType GetType() const override {return ERenderCompositionType::RENDER_COMPOSITION_TYPE_IMAGE;}
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
    IRenderComposition();

    bool Initialize(IRenderCompositionInitializer* initializer);
    bool InitializeWithSurface(RenderCompositionInitializerSurface* initializer);
    bool InitializeWithImage(RenderCompositionInitializerImage* initializer);
    
    bool Recreate(IRenderCompositionInitializer* initializer);

    uint32_t AddLayerRef(IRenderLayerRef* ref);
    std::pair<uint32_t, uint32_t> AddLayerRefs(std::vector<IRenderLayerRef*> &ref);
    IRenderLayerRef* GetLayer(uint32_t idx);

    void StartFrame();
    void Render(float DeltaTime);
    void EndFrame();

    bool Deinitialize();
};