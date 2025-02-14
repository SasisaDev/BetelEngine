#include "Entity.h"

#include "World.h"

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
    if(GetParent<Entity>()) {
        return transform + GetParent<Entity>()->transform;
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