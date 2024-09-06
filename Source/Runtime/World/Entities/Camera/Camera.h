#pragma once

#include <World/Entity.h>

#ifdef EDITOR

class CameraRenderProxy : public EntityRenderProxy
{
public:
    CameraRenderProxy(Entity* DefaultParent) : EntityRenderProxy(DefaultParent) {}
    virtual void Update() override{}
    virtual void Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef) override;
};

#endif

class CameraEntity : public Entity
{

};