#pragma once

#include <memory>

#include <World/Entity.h>
#include <RenderV/Objects/Material.h>

class EntityRenderProxyTest : public EntityRenderProxy
{
    std::shared_ptr<IShader> shader;
    std::shared_ptr<IMaterial> material;
    Entity* Parent;
public:
    EntityRenderProxyTest(Entity* DefaultParent);
    virtual void CreateResources(WorldRenderLayerRef* layerRef) override;
    virtual void Update(WorldRenderLayerRef* layerRef) override{}
    virtual void Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef) override;
};

class EntityTest : public Entity
{
public:

    virtual EntityRenderProxy* SetupRenderProxy(WorldRenderLayerRef* ref) override {return RenderProxy = new EntityRenderProxyTest(this);}

    virtual void Tick(float deltaTime) override;
};