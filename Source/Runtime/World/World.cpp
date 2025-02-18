#include "World.h"
#include <Object/ObjectTypeLibrary.h>

bool ObjWorldType::bRegistered = ObjectTypeLibrary::Get().RegisterObjectType<ObjWorldType>(World::GetStaticType());

World::World()
{
    
}

void World::DestroyEntity(Entity *entity)
{
    assert(entity != nullptr);

    for(int entID = 0; entID < entities.size(); ++entID) {
        // Remove destruction pending entity
        uint32_t destroyEntityId = entity->GetID();
        uint32_t thisEntityId = entities[entID].GetID();
        if(destroyEntityId == thisEntityId) {
            OnEntityDestroyed.Broadcast(entity);
            entities.erase(entities.begin() + entID);
            break;
        }
    }
}

void World::SpawnDefaultEntities()
{
    // TODO: Spawn Default Entities
}

void World::Preinitialize()
{
    for(int i = 0; i < entities.size(); ++i)
    {
        if(entities[i].Get())
        {
            entities[i]->Preinitialize();
        }
    }
}

void World::BeginPlay()
{

}

void World::Tick(float DeltaTime)
{
    timerManager.Tick(DeltaTime * TimeSpeed);

    for(int entID = 0; entID < entities.size(); ++entID) {
        // Remove destruction pending entity
        if(entities[entID].Get()->HasEntityFlag(EntityFlags::BeginDestroy)) {
            OnEntityDestroyed.Broadcast(entities[entID].Get());
            entities.erase(entities.begin() + entID);
            --entID;
            continue;
        }

        entities[entID].Get()->Tick(DeltaTime * TimeSpeed);
    }
}