#pragma once

#include <Object/Object.h>
#include <memory>
#include <GameFramework/Assets/Texture/Texture.h>
#include <Object/ObjectRef.h>

class ObjAtlas : public Object
{
    ObjectRef<ObjTexture> texture;
public:

};

class ObjAtlasType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new ObjAtlas; }
};