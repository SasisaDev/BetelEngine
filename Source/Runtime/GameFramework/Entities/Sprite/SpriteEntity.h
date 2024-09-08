#pragma once

#include <World/Entity.h>
#include <RenderV/Objects/Material.h>

class SpriteRenderProxy : public EntityRenderProxy
{
    IShader* shader;
    IMaterial* material;

    VkImage image;
    VkImageView imageView;
    static VkSampler sampler;
public:
    SpriteRenderProxy(Entity* DefaultParent) : EntityRenderProxy(DefaultParent) {}
    virtual void Update(WorldRenderLayerRef* layerRef) override{}
    virtual void Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef) override;
};

class SpriteEntity : public Entity
{
public:

    virtual EntityRenderProxy* CreateRenderProxy() override {return RenderProxy = new SpriteRenderProxy(this);}
};