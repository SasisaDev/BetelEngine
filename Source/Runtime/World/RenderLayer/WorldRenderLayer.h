#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <RenderV/Layer.h>
#include <RenderV/Objects/Material.h>
#include <RenderV/Objects/Buffers/Buffer.h>

#include <RenderV/Composition.h>

class World;
class Entity;
class EntityRenderProxy;

struct WorldRenderLayerGPUStorage
{
    glm::mat4 ProjectionMatrix;
    // View Matrix should only be used for Rotation
    glm::mat4 ViewMatrix;
    glm::vec2 CameraPosition;
};

struct UpscaleGPUStorage
{
    glm::vec2 AspectRationCompensation = {1, 1};
};

class WorldRenderLayerRef : public IRenderLayerRef
{
protected:
    World* world;
    VkExtent2D viewport;
    VkFormat imageFormat;

    std::vector<EntityRenderProxy*> renderProxies;
    std::vector<EntityRenderProxy*> postRenderProxies;
    bool bRenderPostRenderProxies = true;

    VkImage pixelPerfectDepthImage;
    VkDeviceMemory pixelPerfectDepthImageMemory;
    VkImageView pixelPerfectDepthImageView;
    
    std::vector<VkImage> pixelPerfectImages;
    std::vector<VkDeviceMemory> pixelPerfectImageMemories;
    std::vector<VkImageView> pixelPerfectImageViews;
    std::vector<VkFramebuffer> pixelPerfectImageFramebuffers;
    VkSampler pixelPerfectSampler;

    // GPU Storage Buffer Binding 0
    std::vector<WorldRenderLayerGPUStorage> SceneDataStorages;
    std::vector<Buffer*> SceneDataSSBOs;

    // Upscale Buffer Binding 1
    UpscaleGPUStorage UpscaleDataStorage;
    Buffer* UpscaleDataSSBO;

    friend class WorldRenderLayer;
public:
    WorldRenderLayerRef();
    ~WorldRenderLayerRef();

    virtual bool Initialize(VkDevice device, RenderDependencyList<IRenderLayerRef>& DependencyList) override;
    virtual bool Recreate() override;

    inline Buffer* GetSceneDataBuffer() const {return SceneDataSSBOs[GetParentComposition()->GetCurrentImageIndex()];}

    WorldRenderLayerRef* SubscribeWorldLoad(MulticastDelegate<World*>* delegate);
    WorldRenderLayerRef* SubscribeWorldUnload(MulticastDelegate<World*>* delegate);
    WorldRenderLayerRef* SetWorld(World* newWorld);
    WorldRenderLayerRef* SetViewportSize(VkExtent2D newViewport) {viewport = newViewport; return this;}

    inline World* GetWorld() const {return world;}

    void onWorldLoad(World* loadedWorld);
    void onWorldUnload(World* loadedWorld){}
    void onWorldEntitySpawned(Entity* entity);

    void SetRenderPostRenderProxies(bool render) { bRenderPostRenderProxies = render; }
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

    virtual VkRenderPass GetCompositionCompatRenderPass() const override {return upscaleRenderPass;}

    static std::string GetStaticName() {return "WorldRenderLayer";}

    virtual bool Initialize(VkDevice device) override;

    virtual void Prepare(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer) override;

    virtual void Render(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer) override;

    virtual bool Deinitialize() override;
};