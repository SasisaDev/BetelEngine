#include "Camera.h"
#include <World/World.h>

void CameraEntity::Tick(float deltaTime)
{
    if(bCameraActive) 
    {
        GetWorld()->SetWorldCameraPosition({transform.Location.x, transform.Location.y});
        //TODO: GetWorld()->SetWorldCameraRotation({transform.Rotator.x, transform.Rotator.y});
    }
}