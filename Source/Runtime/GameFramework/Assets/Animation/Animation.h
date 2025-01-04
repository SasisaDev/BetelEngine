#pragma once

#include <Object/Object.h>
#include <memory>
#include <GameFramework/Assets/Atlas/Atlas.h>
#include <Object/ObjectRef.h>

class ObjAnimation : public Object
{
    ObjectRef<ObjAtlas> atlas;
public:

};

class ObjAnimationType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new ObjAnimation; }
};