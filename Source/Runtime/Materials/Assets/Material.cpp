#include "Material.h"

#include <Object/ObjectTypeLibrary.h>

bool ObjMaterialType::bRegistered = ObjectTypeLibrary::Get().RegisterObjectType<ObjMaterialType>("MAT");