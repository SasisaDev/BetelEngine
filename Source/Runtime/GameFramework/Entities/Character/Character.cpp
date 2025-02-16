#include "Character.h"
#include <Object/ObjectTypeLibrary.h>
#include <World/World.h>

bool EntCharacterType::bRegistered = ObjectTypeLibrary::Get().RegisterObjectType<EntCharacterType>(EntCharacter::GetStaticType());

void EntCharacter::Preinitialize()
{
    Entity::Preinitialize();

    // TODO: Make it into one SpawnChild function somehow
    Camera = GetWorld()->Spawn<EntCamera>("Camera");
    Camera->Reparent(this);
}

void EntCharacter::Tick(float deltaTime)
{
    Entity::Tick(deltaTime);
}
