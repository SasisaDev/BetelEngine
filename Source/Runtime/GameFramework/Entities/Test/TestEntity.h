#pragma once

#include <memory>

#include <World/Entity.h>
#include <RenderV/Objects/Shader.h>

class EntityRenderProxyTest : public EntityRenderProxy
{
    std::shared_ptr<IShader> shader;
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

    virtual EntityRenderProxy* CreateRenderProxy() override {return RenderProxy = new EntityRenderProxyTest(this);}
};