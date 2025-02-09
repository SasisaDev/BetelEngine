#pragma once

#include <Object/Object.h>

#include <RenderV/Vulkan/vkloader.h>

#include <set>
#include <concepts>
#include <type_traits>
#include <string>

#include <Math/Transform.h>

class EntityRenderProxy;
class World;
class Editor;

struct EntitySpawnInfo
{
    static EntitySpawnInfo Empty;
    IVec3 Location = {0, 0, 0};
};

enum class EntityFlags : uint8_t {
    // This flag means that Entity will get destroyed next tick
    // It will also remove entity from Object Library. 
    BeginDestroy
};

class Entity : public Object
{
    friend class World;
protected:
    Transform transform;
    // Dynamic entities are testing against dynamic value, like collision
    bool bIsDynamic = false;

    uint8_t entityFlags = 0;

    EntityRenderProxy* RenderProxy;

    std::string DisplayName = "Entity";
public:
    bool Visible = true;

    inline bool IsDynamic() const {return bIsDynamic;}

    virtual EntityRenderProxy* CreateRenderProxy(){return RenderProxy = nullptr;}
    virtual EntityRenderProxy* GetRenderProxy() {return RenderProxy;}

    virtual World* GetWorld();
    
    virtual void BeginPlay() {}
    virtual void Tick(float deltaTime);

    virtual Transform GetTransform();
    virtual const Transform& GetRelativeTransform() const {return transform;}

    virtual IVec3 GetLocation();
    virtual const IVec3& GetRelativeLocation() const {return transform.Location;}

    // TODO
    virtual void SetLocation(const IVec3& loc){}
    virtual void SetRelativeLocation(const IVec3 loc) {transform.Location = loc;}

    inline void SetEntityFlag(EntityFlags flag) {entityFlags |= (uint8_t)flag;}
    inline void ClearEntityFlag(EntityFlags flag) {entityFlags &= ~(uint8_t)flag;}
    inline bool HasEntityFlag(EntityFlags flag) {return (entityFlags & (uint8_t)flag) == (uint8_t)flag;}

    void BeginDestroy() {SetEntityFlag(EntityFlags::BeginDestroy);}

    // Used for testing of collisions and visibility
    virtual Vec3 GetBoundingBox() {return transform.Scale;}

    virtual std::string GetDisplayName() {return DisplayName;} 

    // Editor API
#   ifdef EDITOR
    void PushEditorModes(Editor* editor) {}
    void PopEditorModes(Editor* editor) {}
#   endif
};

class WorldRenderLayerRef;

class EntityRenderProxy
{
public:
    Entity* Parent;

    // Specifies whether entity gets automatically loaded/unloaded from memory
    bool bPersistent = false;
public:
    EntityRenderProxy(Entity* DefaultParent){Parent = DefaultParent;}
    virtual ~EntityRenderProxy(){}
    
    virtual void CreateResources(WorldRenderLayerRef* layerRef){}
    virtual void Update(WorldRenderLayerRef* layerRef) {}
    virtual void Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef) {}
};

template<typename T>
concept EntityClass = std::is_base_of_v<Entity, T>;