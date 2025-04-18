#pragma once

#include <Object/Object.h>

#include <RenderV/Vulkan/vkloader.h>

#include <set>
#include <concepts>
#include <type_traits>
#include <string>

#include <Math/Transform.h>

#ifdef EDITOR
class EditorMode;
#endif

class EntityRenderProxy;
class WorldRenderLayerRef;
class World;
class Editor;
class Entity;

template<typename T>
concept EntityClass = std::is_base_of_v<Entity, T>;

enum class EntityMobilityPolicy : bool
{
    Static = 0,
    Dynamic = 1
};

struct EntitySpawnInfo
{
    static EntitySpawnInfo Empty;
    IVec3 Location = {0, 0, 0};
    bool bTransient = true;
};

enum class EntityFlags : uint8_t {
    // This flag means that Entity will get destroyed next tick
    // It will also remove entity from Object Library. 
    BeginDestroy = (1 << 0),
};

class Entity : public Object
{
    friend class World;
protected:
    Transform transform;
    // Dynamic entities are testing against dynamic value, like collision
    EntityMobilityPolicy mobilityPolicy = EntityMobilityPolicy::Static;

    uint8_t entityFlags = 0;

    EntityRenderProxy* RenderProxy;

    std::string DisplayName = "Entity";
protected:
#   ifdef EDITOR
    EntityRenderProxy* EdRenderProxy;
#   endif
public:
    bool bVisible = true;
    bool bTickable = true;

    inline EntityMobilityPolicy GetMobility() const {return mobilityPolicy;}
    inline void SetMobility(EntityMobilityPolicy newValue) {mobilityPolicy = newValue;}

    inline bool IsTickable() const {return bTickable;}

    /*
     * This functions is getting called by WorldRenderLayerRef on entity spawn.
     * It creates or finds an existing RenderProxy, potentially registers itself in batch
     * 
     * Return pointer to new created EntityRenderProxy if it was created, nullptr otherwise 
     */
    virtual EntityRenderProxy* SetupRenderProxy(WorldRenderLayerRef* ref){return RenderProxy = nullptr;}
    virtual EntityRenderProxy* GetRenderProxy() {return RenderProxy;}

    virtual World* GetWorld();

    // Used to spawn children alongside main entity
    // This functions is only called on SpawnEntity or AddEntity
    virtual void SpawnChildren(World* world){}
    
    virtual void PostInit(){}
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

    //~Reflection API start
#   ifdef EDITOR 
       virtual PropertyContainer GetEditorReflectedProperties() override;
#   endif
    //~Reflection API stop

    // Editor API
#   ifdef EDITOR
    virtual std::vector<EditorMode*> GetEditorModes() {return {};}
#   endif
};

class WorldRenderLayerRef;

class EntityRenderProxy
{
public:
    Entity* Parent;
    bool bIsBatch = false;
public:
    EntityRenderProxy(Entity* DefaultParent){Parent = DefaultParent;}
    virtual ~EntityRenderProxy(){}

    constexpr inline bool IsBatch() const {return bIsBatch;}

    static std::string GetStaticType() {return "ENT";}
    
    virtual void CreateResources(WorldRenderLayerRef* layerRef){}
    virtual void Update(WorldRenderLayerRef* layerRef) {}
    virtual void Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef) {}
};

class EntityType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new Entity; }
    virtual bool ShowInEditor() override { return true; }
    virtual bool IsEntity() override { return true; }
    virtual std::string_view DisplayName() override {return "Entity";}
};
