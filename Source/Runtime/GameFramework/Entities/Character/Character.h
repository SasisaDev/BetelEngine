#pragma once

#include <World/Entity.h>
#include <Object/ObjectRef.h>
#include <World/Entities/Camera/Camera.h>
#include <GameFramework/Entities/Sprite/SpriteEntity.h>

class EntCharacter : public Entity
{
    ObjectRef<EntCamera> Camera;
    ObjectRef<EntSprite> Sprite;

    IVec2 MovingVector = {};
public:
    EntCharacter() {
        DisplayName = "Character";
    }

    bool bAutoPossess = false;

    static std::string GetStaticType() {return "CHAR";}

    EntCamera* GetCamera() const {return Camera.Get();}

    virtual void SpawnChildren(World* world) override;
    virtual void PostInit() override;
    virtual void Tick(float deltaTime) override;

    virtual void HandleInput(InputEvent& event);
};

class EntCharacterType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new EntCharacter; }
    virtual bool ShowInEditor() override { return false; }
    virtual std::string_view DisplayName() override {return "Character";}
};