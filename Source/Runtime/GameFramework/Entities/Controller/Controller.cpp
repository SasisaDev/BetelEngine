#include "Controller.h"
#include <Object/ObjectTypeLibrary.h>

bool EntControllerType::bRegistered = ObjectTypeLibrary::Get().RegisterObjectType<EntControllerType>(EntController::GetStaticType());

void EntController::Possess(EntCharacter *possessCharacter)
{
    assert(possessCharacter != nullptr);

    if(possessCharacter != nullptr)
    {
        character = possessCharacter;
    }
}

void EntController::Tick(float deltaTime)
{
    Entity::Tick(deltaTime);
}
