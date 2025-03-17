#include "PlayerController.h"
#include <Object/ObjectTypeLibrary.h>
#include <World/World.h>

bool EntPlayerControllerType::bRegistered = ObjectTypeLibrary::Get().RegisterObjectType<EntPlayerControllerType>(EntPlayerController::GetStaticType());

void EntPlayerController::PostInit()
{
    for(ObjectRef<Entity> entity : GetWorld()->GetEntities())
    {
        if(EntCharacter* character = dynamic_cast<EntCharacter*>(entity.Get()))
        {
            if(character->bAutoPossess)
            {
                this->Possess(character);
            }
        }
    }
}

void EntPlayerController::Possess(EntCharacter *possessCharacter)
{
    EntController::Possess(possessCharacter);

    if(possessCharacter != nullptr)
    {
        camera = possessCharacter->GetCamera();
    }
}

void EntPlayerController::Unpossess()
{
    EntController::Unpossess();
    camera = nullptr;
}

void EntPlayerController::OverrideCamera(EntCamera *overrideCamera)
{
    camera = overrideCamera;
}

void EntPlayerController::StopCameraOverride()
{
    if(character != nullptr)
    {
        camera = character->GetCamera();
    }
}

void EntPlayerController::Tick(float deltaTime)
{
    EntController::Tick(deltaTime);

    if(camera) 
    {
        GetWorld()->GetSceneView().ViewOrigin = {camera->GetTransform().Location.x, camera->GetTransform().Location.y};
        //TODO: GetWorld()->SetWorldCameraRotation({transform.Rotator.x, transform.Rotator.y});
    }
}

void EntPlayerController::HandleInput(InputEvent &event)
{
    if(character != nullptr)
    {
        character->HandleInput(event);
    }
}
