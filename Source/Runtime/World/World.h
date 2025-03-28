#pragma once
#include "Entity.h"
#include "SceneView.h"

#include <vector>
#include <unordered_set>
#include <string>
#include <Math/Transform.h>
#include <Delegate/Delegate.h>
#include <Timer/Timer.h>
#include <Input/InputEvent.h>
#include <Object/ObjectLibrary.h>
#include <Object/ObjectRef.h>
#include <Engine/Engine.h>

class WorldRenderLayerRef;
class EntPlayerController;

class World : public Object
{
    friend class WorldRenderLayer;
    friend class WorldRenderLayerRef;

    EntPlayerController* playerController = nullptr;
protected:
    TimerManager timerManager;

    SceneView sceneView;

    std::vector<ObjectRef<Entity>> entities;

    bool BackgroundGradient = false;
    Vec3 BackgroundColor;
    std::pair<Vec3, Vec3> BackgroundGradientColor;

    std::string Name = "Persistent World";

    float TimeSpeed = 1.0;
public:
    World();

    static std::string GetStaticType() {return "WORLD";}

    MulticastDelegate<Entity*> OnEntitySpawned;
    MulticastDelegate<Entity*> OnEntityDestroyed;

    TimerManager& GetTimerManager() { return timerManager; }

    SceneView& GetSceneView() {return sceneView;}

    void SetBackgroundColor(const Vec3& color) {BackgroundColor = color;}

    void SetTimeSpeed(float timeSpeed) { TimeSpeed = timeSpeed; }
    float GetTimeSpeed() const { return TimeSpeed; }

    void SetWorldName(const std::string& name) {Name = name;}
    std::string GetWorldName() const {return Name;}

    // TODO: All objects, including entities, must be registered in ObjectLibrary and passed around in ObjectRef<>
    template<EntityClass EntityType>
    EntityType* Spawn(const std::string& name, const EntitySpawnInfo& spawnInfo = EntitySpawnInfo::Empty)
    {
        EntityType* spawnedEntity = GEngine->GetObjectLibrary()->CreateObject<EntityType>(name, spawnInfo.bTransient);
        spawnedEntity->DisplayName = name;
        spawnedEntity->Reparent(this);
        spawnedEntity->transform.Location = spawnInfo.Location;
        entities.emplace_back(spawnedEntity);

        spawnedEntity->SpawnChildren(this);
        spawnedEntity->PostInit();
        OnEntitySpawned.Broadcast(spawnedEntity);

        return spawnedEntity;
    }

    Entity* SpawnFromTypeID(const std::string& typeID, const std::string& name, const EntitySpawnInfo& spawnInfo = EntitySpawnInfo::Empty)
    {
        Entity* spawnedEntity = dynamic_cast<Entity*>(GEngine->GetObjectLibrary()->CreateObjectFromTypeID(typeID, name, spawnInfo.bTransient));
        spawnedEntity->DisplayName = name;
        spawnedEntity->Reparent(this);
        spawnedEntity->transform.Location = spawnInfo.Location;
        entities.emplace_back(spawnedEntity);

        spawnedEntity->SpawnChildren(this);
        spawnedEntity->PostInit();
        OnEntitySpawned.Broadcast(spawnedEntity);

        return spawnedEntity;
    }

    // Adds a pre-initialized Entity to the world
    void AddEntity(Entity* entity, const EntitySpawnInfo& spawnInfo = EntitySpawnInfo::Empty)
    {
        assert(entity != nullptr);

        // Entity must not be nullptr
        if(entity == nullptr) {
            return;
        }

        // Don't add entity if it's already added
        for(const ObjectRef<Entity>& existingEntity : entities)
        {
            if(existingEntity.GetID() == entity->GetID()) {
                return;
            }
        }

        entity->Reparent(this);
        entity->transform.Location = spawnInfo.Location;
        entities.emplace_back(entity);

        entity->SpawnChildren(this);
        entity->PostInit();
        OnEntitySpawned.Broadcast(entity);
    }

    // Destroys entity on this tick
    // It can have negative effect
    // You should never use this function
    void DestroyEntity(Entity* entity);

    inline const std::vector<ObjectRef<Entity>>& GetEntities() const {return entities;}

    // Gets called first when world's loaded.
    // Creates Player Controller and other default entities
    virtual void SpawnDefaultEntities();

    // Gets called right after SpawnDefaultEntities()
    // Allows entities to initialize their data that should be ready on BeginPlay
    virtual void PostInit();

    virtual void BeginPlay();

    virtual void Tick(float DeltaTime);

    virtual void PushInputEvent(InputEvent &input);
};

class ObjWorldType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new World; }
    virtual std::string_view DisplayName() override {return "World";}
};