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

class IRenderComposition;

struct IRenderCompositionInitializer
{
    virtual ERenderCompositionType GetType() const {return ERenderCompositionType::RENDER_COMPOSITION_TYPE_NONE;}

    virtual void Initialize(IRenderComposition* composition){}
};

struct RenderCompositionInitializerSurface : public IRenderCompositionInitializer
{
    VkSurfaceKHR surface;
    VkExtent2D extent;
    VkFormat format;
    
    // Functions

    virtual ERenderCompositionType GetType() const override {return ERenderCompositionType::RENDER_COMPOSITION_TYPE_SURFACE;}

    virtual void Initialize(IRenderComposition* composition) override;
};

struct RenderCompositionInitializerImage : public IRenderCompositionInitializer
{
    virtual ERenderCompositionType GetType() const override {return ERenderCompositionType::RENDER_COMPOSITION_TYPE_IMAGE;}

    virtual void Initialize(IRenderComposition* composition) override;
};

class IRenderComposition
{
    friend class IRenderCompositionInitializer;
    friend class RenderCompositionInitializerImage;
    friend class RenderCompositionInitializerSurface;
protected:
    VkSwapchainKHR swapchain;
    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
    std::vector<VkFramebuffer> framebuffers;
    VkSurfaceKHR surface;

    VkFormat imageFormat;
    VkExtent2D extent;

    bool bIsPresentable = false;

    uint32_t targetImageId = 0;

    std::vector<IRenderLayerRef*> Layers;
    RenderDependencyList<IRenderLayerRef> DependencyList;

    ERenderCompositionType compositionType;
public:
    IRenderComposition();

    bool Initialize(IRenderCompositionInitializer* initializer);
    
    bool Recreate(IRenderCompositionInitializer* initializer);

    uint32_t AddLayerRef(IRenderLayerRef* ref);
    std::pair<uint32_t, uint32_t> AddLayerRefs(std::vector<IRenderLayerRef*> &ref);
    IRenderLayerRef* GetLayer(uint32_t idx);
    inline const std::vector<IRenderLayerRef*>& GetLayerRefs() const { return Layers; }

    VkSurfaceKHR GetSurface() const {return surface;}

    void StartFrame();
    void Render(float DeltaTime);
    void EndFrame();

    bool Deinitialize();
};