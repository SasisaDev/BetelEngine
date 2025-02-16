#pragma once

#include <World/Entity.h>
#include <Object/ObjectRef.h>
#include <World/Entities/Camera/Camera.h>

class EntCharacter : public Entity
{
    ObjectRef<EntCamera> Camera;
public:
    EntCharacter() {
        DisplayName = "Character";
    }

    static std::string GetStaticType() {return "CHAR";}

    EntCamera* GetCamera() const {return Camera.Get();}

    virtual void Preinitialize() override;
    virtual void Tick(float deltaTime) override;
};

class EntCharacterType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new EntCharacter; }
    virtual bool ShowInEditor() override { return false; }
    virtual std::string_view DisplayName() override {return "Character";}
};