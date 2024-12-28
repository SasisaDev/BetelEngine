#pragma once

#include <World/Entity.h>
#include <RenderV/Objects/Material.h>
#include <Materials/Assets/ShaderAsset.h>
#include <Materials/Assets/MaterialAsset.h>
#include <Assets/AssetSmartPointers.h>
#include <GameFramework/Assets/Texture2D/Texture2D.h>

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
    AssetPtr<MaterialAsset> material;
    AssetPtr<Texture2D> texture;

    SpriteEntity() {
        DisplayName = "Sprite";
    }

    virtual EntityRenderProxy* CreateRenderProxy() override {return RenderProxy = new SpriteRenderProxy(this);}
};