#include "Entity.h"

void Entity::Tick(float deltaTime)
{

}

Transform Entity::GetTransform()
{
    if(Parent) {
        return transform + Parent->transform;
    } else {
        return transform;
    }
}

Vec3 Entity::GetLocation()
{
    if(Parent) {
        return transform.Location + Parent->transform.Location;
    } else {
        return transform.Location;
    }
}