#pragma once

#include <memory>

#include <RenderV/Layer.h>
#include <RenderV/Objects/Material.h>

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
    VkPipeline upscalePipeline;
    VkPipelineLayout upscalePipelineLayout;
    void CreateUpscalePipeline();

    std::shared_ptr<IShader> upscaleShader;
    std::shared_ptr<IMaterial> upscaleMaterial;

    void CreateUpscaleMaterial();

    friend class WorldRenderLayerRef;

    VkRenderPass upscaleRenderPass;
public:
    static IRenderLayerRef* CreateRef() {return new WorldRenderLayerRef;}

    static std::string GetName() {return "WorldRenderLayer";}

    virtual bool Initialize(VkDevice device) override;

    virtual void Prepare(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer) override;

    virtual void Render(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer) override;

    virtual bool Deinitialize() override;
};