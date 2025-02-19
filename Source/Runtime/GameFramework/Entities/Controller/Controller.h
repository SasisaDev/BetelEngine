#pragma once

#include <GameFramework/Entities/Character/Character.h>

class EntController : public Entity
{
protected:
    EntCharacter* character;
public:
    EntController() {
        DisplayName = "Controller";
    }

    static std::string GetStaticType() {return "CTRL";}

    virtual void Possess(EntCharacter* possessCharacter);
    virtual EntCharacter* GetPossessed() const {return character;}
    virtual void Unpossess() { character = nullptr; }

    virtual void Tick(float deltaTime) override;
};

class EntControllerType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new EntController; }
    virtual bool ShowInEditor() override { return false; }
    virtual std::string_view DisplayName() override {return "Controller";}
};