#include "Entity.h"

#include "World.h"

#include <Object/ObjectTypeLibrary.h>

bool EntityType::bRegistered = ObjectTypeLibrary::Get().RegisterObjectType<EntityType>(Entity::GetStaticType());

EntitySpawnInfo EntitySpawnInfo::Empty = EntitySpawnInfo();

void Entity::Tick(float deltaTime)
{

}

World* Entity::GetWorld() 
{
    return GetParent<World>();
}

Transform Entity::GetTransform()
{
    if(Entity* parent = GetParent<Entity>()) {
        return transform + parent->transform;
    } else {
        return transform;
    }
}

IVec3 Entity::GetLocation()
{
    if(Entity* ent = GetParent<Entity>()) {
        return transform.Location + ent->transform.Location;
    } else {
        return transform.Location;
    }
}

#ifdef EDITOR 
PropertyContainer Entity::GetEditorReflectedProperties()
{
    return  Object::GetEditorReflectedProperties().
            PushPropertyString("Name", &DisplayName);
}
#endif