#include "Sprite.h"

#include <Object/ObjectTypeLibrary.h>
#include <GameFramework/Entities/Sprite/SpriteEntity.h>
#include <Engine/Engine.h>

bool ObjSpriteType::bRegistered = ObjectTypeLibrary::Get().RegisterObjectType<ObjSpriteType>("SPRITE");

Entity* ObjSpriteType::CreateWorldEntity(Object* Object)
{
    return GEngine->GetObjectLibrary()->CreateObject<EntSprite>("Sprite", false);
}