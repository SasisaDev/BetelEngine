#pragma once

#include <World/Entity.h>


// TODO
class EntityRenderProxyTest : public EntityRenderProxy
{

public:
    EntityRenderProxyTest(Entity* DefaultParent, void* sprite) : EntityRenderProxy(DefaultParent) {}
    virtual void Update() override{}
    virtual void Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef) override;
};