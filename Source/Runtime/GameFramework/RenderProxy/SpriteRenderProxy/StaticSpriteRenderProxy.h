#pragma once

#include <World/Entity.h>


// TODO
class StaticSpriteRenderProxy : public EntityRenderProxy
{

public:
    SpriteRenderProxy(Entity* DefaultParent, void* sprite) : EntityRenderProxy(DefaultParent) {}
    virtual void Update() override{}
    virtual void Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef) override;
};