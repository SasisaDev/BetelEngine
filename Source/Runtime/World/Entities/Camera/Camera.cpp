#include "Camera.h"
#include <World/World.h>

void EntCamera::Tick(float deltaTime)
{
    if(bCameraActive) 
    {
        GetWorld()->SetWorldCameraPosition({transform.Location.x, transform.Location.y});
        //TODO: GetWorld()->SetWorldCameraRotation({transform.Rotator.x, transform.Rotator.y});
    }

    // TODO: Remove testing code
    if(transform.Location.y >= 145) {
        transform.Location.y = -35;
    }
    if(transform.Location.x >= 295) {
        transform.Location.x = -25;
    }

    transform.Location.y += 1;
    transform.Location.x += 1;
}