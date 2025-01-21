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
    std::vector<IRenderLayerRef*> layerRefs;
    
    virtual ERenderCompositionType GetType() const {return ERenderCompositionType::RENDER_COMPOSITION_TYPE_NONE;}

    virtual void Initialize(IRenderComposition* composition){}
};

struct RenderCompositionInitializerSurface : public IRenderCompositionInitializer
{
    VkSurfaceKHR surface = VK_NULL_HANDLE;
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
    bool bPauseRender = false;

    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
    std::vector<VkFramebuffer> framebuffers;
    VkSurfaceKHR surface;

    VkFormat imageFormat;
    VkOffset2D offset = {0, 0};
    VkExtent2D extent;

    VkRect2D GameViewport = {0};

    std::vector<VkSemaphore> aquireSemaphores;

    bool bIsPresentable = false;

    uint32_t targetImageId = 0;

    std::vector<IRenderLayerRef*> Layers;
    RenderDependencyList<IRenderLayerRef> DependencyList;

    ERenderCompositionType compositionType;

    bool bCompositionDirty = false;
public:
    IRenderComposition();

    inline ERenderCompositionType GetType() const {return compositionType;}
    
    inline VkSemaphore GetAquireSemaphore() const {return aquireSemaphores[targetImageId];}

    inline size_t GetFramesInFlight() const {return 2;}

    void Pause() {bPauseRender = true;}
    void Unpause() {bPauseRender = false;}

    bool Initialize(IRenderCompositionInitializer* initializer);
    
    bool Recreate(IRenderCompositionInitializer* initializer);

    uint32_t AddLayerRef(IRenderLayerRef* ref);
    std::pair<uint32_t, uint32_t> AddLayerRefs(std::vector<IRenderLayerRef*> &ref);
    IRenderLayerRef* GetLayer(uint32_t idx) {return Layers[idx];}
    template <typename LayerRefT>
    int GetLayerIDFromType() {
        for(int i = 0; i < Layers.size(); i++) {
            if(dynamic_cast<LayerRefT*>(Layers[i])) {
                return i;
            }
        }
        return -1;
    }
    inline const std::vector<IRenderLayerRef*>& GetLayerRefs() const { return Layers; }

    inline VkSurfaceKHR GetSurface() const {return surface;}
    inline VkSwapchainKHR GetSwapchain() const {return swapchain;}

    // Mark composition Dirty, leading to it's recreation
    inline void SetDirty() {bCompositionDirty = true;}
    
    /*
     * Some elements of rendering may go beyond the game viewport
     * For such cases Rendering Viewport and Game Viewport are separate
     * This is mostly used by Editor features, but it will work in Runtime as well 
    */
    inline VkRect2D GetGameViewport() const {return GameViewport;}
    inline VkOffset2D GetOffset() const {return offset;}
    inline VkExtent2D GetExtent() const {return extent;}
    inline VkFramebuffer GetCurrentFramebuffer() const {return framebuffers[targetImageId];}
    inline VkImage GetCurrentImage() const {return images[targetImageId];}
    inline uint32_t GetCurrentImageIndex() const {return targetImageId;}

    inline void SetGameViewport(VkRect2D newValue) 
    {
        if( newValue.extent.width != GameViewport.extent.width || newValue.extent.height != GameViewport.extent.height ||
            newValue.offset.x != GameViewport.offset.x || newValue.offset.y != GameViewport.offset.y) 
        {
            GameViewport = newValue; 
            MakeLayersDirty();
        }
    }
    inline void SetOffset(VkOffset2D newValue) {offset = newValue;}
    inline void SetExtent(VkExtent2D newValue) {extent = newValue;}

    // Forces layer refs to recreate their resources 
    // Should not be called directly due to risk in interrupting CmdBuffer recording
    void NotifyLayersRecreateResources();

    // Set all layers as dirty
    void MakeLayersDirty();

    void StartFrame(VkCommandBuffer cmdBuffer);
    void Render(VkCommandBuffer cmdBuffer);
    void EndFrame(VkCommandBuffer cmdBuffer){}

    bool Deinitialize();
};