#pragma once

#include <World/Entity.h>
#include <RenderV/Objects/Material.h>
#include <Materials/Assets/ShaderAsset.h>
#include <Materials/Assets/MaterialAsset.h>
#include <Assets/AssetSmartPointers.h>
#include <GameFramework/Assets/Texture2D/Texture2D.h>

#include <AssetLoader/AssetLoader.h>
#include <RenderV/Objects/Images/SamplerTexture.h>

#include <GameFramework/Assets/Sprite/Sprite.h>
#include <Object/ObjectRef.h>

#include <Math/Vertex.h>

class SpriteRenderProxy : public EntityRenderProxy
{
    std::shared_ptr<IShader> shader;
    std::shared_ptr<IMaterial> material;
    std::shared_ptr<ISamplerTexture> texture;

    static Buffer* vertexBuffer;
    static Buffer* indexBuffer;
public:
    SpriteRenderProxy(Entity* DefaultParent) : EntityRenderProxy(DefaultParent) {}
    virtual void CreateResources(WorldRenderLayerRef* layerRef) override;
    virtual void Update(WorldRenderLayerRef* layerRef) override{}
    virtual void Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef) override;

    
};

class SpriteEntity : public Entity
{
    ObjectRef<ObjSprite> sprite;
public:
    AssetPtr<MaterialAsset> material;
    //AssetPtr<Texture2D> texture;


    SpriteEntity() {
        DisplayName = "Sprite";
    }

    virtual void SetSprite(ObjectRef<ObjSprite>& newSprite) {
        sprite = newSprite;
        // TODO: Change object's size and other transforms to match the sprite to enforce pixel-perfect look 
    }

    virtual EntityRenderProxy* CreateRenderProxy() override {return RenderProxy = new SpriteRenderProxy(this);}
};