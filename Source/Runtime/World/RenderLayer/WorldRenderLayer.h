#pragma once

#include <RenderV/Layer.h>

class World;

class WorldRenderLayerRef : public IRenderLayerRef
{
protected:
    World* world;
    VkExtent2D viewport;
    VkFormat imageFormat;

    std::vector<VkImage> pixelPerfectImages;
    std::vector<VkDeviceMemory> pixelPerfectImageMemories;
    std::vector<VkImageView> pixelPerfectImageViews;
    std::vector<VkFramebuffer> pixelPerfectImageFramebuffers;

    friend class WorldRenderLayer;
public:
    WorldRenderLayerRef();

    virtual bool Initialize(VkDevice device, RenderDependencyList<IRenderLayerRef>& DependencyList) override;

    WorldRenderLayerRef* SetWorld(World* newWorld);
    WorldRenderLayerRef* SetViewportSize(VkExtent2D newViewport) {viewport = newViewport; return this;}

    void onWorldLoad(World* loadedWorld){}
};

class WorldRenderLayer : public IRenderLayer
{
    
public:
    static IRenderLayerRef* CreateRef() {return new WorldRenderLayerRef;}

    virtual std::string GetName() const override {return "WorldRenderLayer";}

    virtual bool Initialize(VkDevice device) override;

    virtual void Prepare(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer) override;

    virtual void Render(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer) override;

    virtual bool Deinitialize() override;
};