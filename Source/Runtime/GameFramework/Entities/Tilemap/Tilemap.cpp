#include "Tilemap.h"
#include <Object/ObjectLibrary.h>

bool TilemapEntityType::bRegistered = ObjectLibrary::Get().RegisterObjectType<TilemapEntityType>("TILEMAP");