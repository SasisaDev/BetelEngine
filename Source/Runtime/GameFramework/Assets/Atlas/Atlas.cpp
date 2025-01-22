#include "Atlas.h"

#include <Object/ObjectTypeLibrary.h>
#include <Object/FieldContainer.h>

bool ObjAtlasType::bRegistered = ObjectTypeLibrary::Get().RegisterObjectType<ObjAtlasType>("ATLAS");

const IVec4& ObjAtlas::GetSpriteRect(uint16_t id) const 
{
    if(sprites.contains(id)) { 
        return sprites.at(id); 
    } else { 
        return {0,0,0,0};
    }
}

void ObjAtlas::SetSpriteRect(uint16_t id, const IVec4& rect)
{
    sprites[id] = rect;    
}

void ObjAtlas::SetSpriteRect(uint16_t id, const IVec4&& rect)
{
    sprites[id] = rect;
}

void ObjAtlas::RemoveSpriteRect(uint16_t id)
{
    sprites.erase(id);
}

void ObjAtlas::Serialize(FieldContainer& cont)
{
    if(cont.IsSaving())
    {
        cont.SetObject("tex", texture.GetID());
    } 
    else 
    {
        texture = ObjectRef<ObjTexture>(cont.GetObject("tex"));
    }
}