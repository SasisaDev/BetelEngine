#pragma once
#include "Entity.h"

#include <vector>
#include <string>
#include <Math/Transform.h>
#include <Delegate/Delegate.h>
#include <Timer/Timer.h>
#include <Input/InputEvent.h>
#include <Object/ObjectLibrary.h>
#include <Object/ObjectRef.h>
#include <Engine/Engine.h>

class WorldRenderLayerRef;

class World : public Object
{
    friend class WorldRenderLayer;
    friend class WorldRenderLayerRef;
protected:
    TimerManager timerManager;

    WorldRenderLayerRef *renderLayer = nullptr;

    std::vector<ObjectRef<Entity>> entities;

    bool BackgroundGradient = false;
    Vec3 BackgroundColor;
    std::pair<Vec3, Vec3> BackgroundGradientColor;
    
    IVec2 CameraPosition = {};
    IVec2 CameraRotation = {};
    float CameraNearPlane = -100;
    float CameraFarPlane = 100;

    std::string Name = "Persistent World";
public:
    World();

    MulticastDelegate<Entity*> OnEntitySpawned;
    MulticastDelegate<Entity*> OnEntityDestroyed;

    TimerManager& GetTimerManager() { return timerManager; }

    WorldRenderLayerRef* GetWorldRenderLayerRef() const {return renderLayer;}

    void SetBackgroundColor(const Vec3& color) {BackgroundColor = color;}

    void SetWorldName(std::string name) {Name = name;}
    std::string GetWorldName() const {return Name;}

    // TODO: All objects, including entities, must be registered in ObjectLibrary and passed around in ObjectRef<>
    template<EntityClass EntityType>
    EntityType* Spawn(std::string name, const EntitySpawnInfo& spawnInfo = EntitySpawnInfo::Empty)
    {
        EntityType* spawnedEntity = GEngine->GetObjectLibrary()->CreateObject<EntityType>(name);
        spawnedEntity->DisplayName = name;
        spawnedEntity->Reparent(this);
        spawnedEntity->transform.Location = spawnInfo.Location;
        entities.emplace_back(spawnedEntity);

        OnEntitySpawned.Broadcast(spawnedEntity);

        return spawnedEntity;
    }

    inline const std::vector<ObjectRef<Entity>>& GetEntities() const {return entities;}

    void SetWorldCameraPosition(IVec2 pos) {CameraPosition = pos;}
    void SetWorldCameraRotation(IVec2 rot) {CameraRotation = rot;}
    IVec2& GetWorldCameraPosition() {return CameraPosition;}
    IVec2& GetWorldCameraRotation() {return CameraRotation;}

    virtual void BeginPlay();

    virtual void Tick(float DeltaTime);

    virtual void PushInputEvent(InputEvent &input) {/*TODO World Input Events*/}
};