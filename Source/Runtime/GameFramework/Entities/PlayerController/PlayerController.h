#pragma once

#include <GameFramework/Entities/Controller/Controller.h>

class EntCamera;

class EntPlayerController : public EntController
{
    EntCamera* camera;
public:
    EntPlayerController() {
        DisplayName = "Player Controller";
    }

    static std::string GetStaticType() {return "PLYCTRL";}

    virtual void PostInit() override;

    virtual void Possess(EntCharacter* possessCharacter) override;
    virtual void Unpossess() override;

    virtual void OverrideCamera(EntCamera* overrideCamera);
    virtual void StopCameraOverride();

    virtual void Tick(float deltaTime) override;

    virtual void HandleInput(InputEvent& event);
};

class EntPlayerControllerType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new EntPlayerController; }
    virtual bool ShowInEditor() override { return false; }
    virtual std::string_view DisplayName() override {return "Player Controller";}
};