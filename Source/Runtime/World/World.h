#pragma once
#include "Entity.h"

#include <vector>
#include <string>
#include <Math/Transform.h>
#include <Delegate/Delegate.h>

class World : public Object
{
    friend class WorldRenderLayer;
protected:
    std::vector<Entity*> entities;

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

    void SetBackgroundColor(const Vec3& color) {BackgroundColor = color;}

    void SetWorldName(std::string name) {Name = name;}
    std::string GetWorldName() const {return Name;}

    template<EntityClass EntityType>
    EntityType* Spawn(std::string name, const EntitySpawnInfo& spawnInfo)
    {
        EntityType* spawnedEntity = new EntityType();
        spawnedEntity->DisplayName = name;
        spawnedEntity->Reparent(this);
        entities.push_back(spawnedEntity);

        OnEntitySpawned.Broadcast(spawnedEntity);

        return spawnedEntity;
    }

    inline std::vector<Entity*> GetEntities() const {return entities;}

    void SetWorldCameraPosition(IVec2 pos) {CameraPosition = pos;}
    void SetWorldCameraRotation(IVec2 rot) {CameraRotation = rot;}

    virtual void StartPlay();

    virtual void Tick(float DeltaTime);
};