#pragma once

#include <World/Entity.h>

#ifdef EDITOR

class CameraRenderProxy : public EntityRenderProxy
{
public:
    CameraRenderProxy(Entity* DefaultParent) : EntityRenderProxy(DefaultParent) {}
    virtual void Update(WorldRenderLayerRef* layerRef) override{}
    virtual void Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef) override;
};

#endif

class CameraEntity : public Entity
{
protected:
    bool bCameraActive = false;
public:
    float CameraNearPlane = -100;
    float CameraFarPlane = 100;

    virtual void Tick(float deltaTime) override;

    // TODO: Make uniqueness check
    void SetCameraActive() {
        bCameraActive = true;
    }
};