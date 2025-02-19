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

    SetRelativeLocation({GetRelativeLocation().x + MovingVector.x, GetRelativeLocation().y + MovingVector.y, 0});
}

void EntCharacter::HandleInput(InputEvent &event)
{

    if(event.KeyName == "W") {
        MovingVector.y = 1 * !event.IsUp;
    } else if(event.KeyName == "S") {
        MovingVector.y = -1 * !event.IsUp;
    }
    
    if(event.KeyName == "A") {
        MovingVector.x = -1 * !event.IsUp;
    } else if(event.KeyName == "D") {
        MovingVector.x = 1 * !event.IsUp;
    }
}
