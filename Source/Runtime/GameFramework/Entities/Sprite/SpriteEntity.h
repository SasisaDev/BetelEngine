#pragma once

#include <World/Entity.h>
#include <RenderV/Objects/Material.h>
#include <Materials/Assets/Shader.h>
#include <Materials/Assets/Material.h>
#include <Assets/AssetSmartPointers.h>
#include <GameFramework/Assets/Texture/Texture.h>

#include <AssetLoader/AssetLoader.h>
#include <RenderV/Objects/Images/SamplerTexture.h>

#include <GameFramework/Assets/Sprite/Sprite.h>
#include <GameFramework/Assets/Atlas/Atlas.h>
#include <Object/ObjectRef.h>

#include <Math/Vertex.h>

#include <memory>

struct SpriteEntityPushConstants
{
    glm::ivec2 Position = {0, 0};
    glm::ivec2 Size = {1, 1};
    glm::int32_t Rotation = 0;
    glm::int32_t Depth = 0;
};

// TODO: Add Batch rendering
class SpriteRenderProxy : public EntityRenderProxy
{
    std::unique_ptr<IShader> shader;
    std::unique_ptr<IMaterial> material;
    std::unique_ptr<ISamplerTexture> texture;

    SpriteEntityPushConstants constants;

    std::unique_ptr<Buffer> vertexBuffer;
    std::unique_ptr<Buffer> indexBuffer;
public:
    SpriteRenderProxy(Entity* DefaultParent) : EntityRenderProxy(DefaultParent) {}
    virtual void CreateResources(WorldRenderLayerRef* layerRef) override;
    virtual void Update(WorldRenderLayerRef* layerRef) override{}
    virtual void Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef) override;
};

class EntSprite : public Entity
{
    ObjectRef<ObjSprite> sprite;
    ObjectRef<ObjAtlas> atlas;
    uint16_t spriteID = 0;
public:
    ObjectRef<ObjMaterial> material;
    //AssetPtr<Texture2D> texture;

    EntSprite() {
        DisplayName = "Sprite";
    }

    virtual void MakeSizeMatchTexture();

    virtual void SetSprite(ObjectRef<ObjSprite>& newSprite) {
        sprite = newSprite;
        MakeSizeMatchTexture();
    }

    virtual void SetAtlas(ObjectRef<ObjAtlas>& newAtlas) {
        atlas = newAtlas;
        MakeSizeMatchTexture();
    }

    virtual void SetAtlasRect(uint16_t id) {
        spriteID = id;
        MakeSizeMatchTexture();
    }

    static std::string GetStaticType() {return "ENTSPRITE";}

    virtual void Tick(float deltaTime) override;

    virtual EntityRenderProxy* SetupRenderProxy(WorldRenderLayerRef* ref) override {return RenderProxy = new SpriteRenderProxy(this);}

    // TODO: Make it real, Make it Faster, maybe memoize
    virtual Vec3 GetBoundingBox() override {return transform.Scale * Vec3(32, -32, 1);}

        // Editor API
#   ifdef EDITOR
    virtual std::vector<EditorMode*> GetEditorModes() override;

    virtual PropertyContainer GetEditorReflectedProperties() override;
#   endif
};

class EntSpriteType : public EntityType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new EntSprite; }
    virtual std::string_view DisplayName() override {return "Sprite Entity";}
};
