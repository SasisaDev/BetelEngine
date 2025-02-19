#include "Character.h"
#include <Object/ObjectTypeLibrary.h>
#include <World/World.h>

bool EntCharacterType::bRegistered = ObjectTypeLibrary::Get().RegisterObjectType<EntCharacterType>(EntCharacter::GetStaticType());

void EntCharacter::SpawnChildren(World *world)
{
    Entity::SpawnChildren(world);

    Camera = world->Spawn<EntCamera>("Camera");
    Camera->Reparent(this);
}

void EntCharacter::PostInit()
{
    Entity::PostInit();
}

void EntCharacter::Tick(float deltaTime)
{
    Entity::Tick(deltaTime);
}
