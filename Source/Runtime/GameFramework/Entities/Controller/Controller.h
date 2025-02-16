#pragma once

#include <World/Entity.h>

class EntController : public Entity
{
    
public:
    EntController() {
        DisplayName = "Controller";
    }

    static std::string GetStaticType() {return "CTRL";}

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