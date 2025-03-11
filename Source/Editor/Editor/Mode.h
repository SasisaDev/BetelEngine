#pragma once
#include <Input/InputEvent.h>
#include <i18n/Text.h>
#include <imgui/imgui_internal.h>

class EditorMode
{
    Text Name;
public:
    EditorMode(const std::string_view& name) : Name(name){}

    Text &GetName() {return Name;}

    virtual void OnEnabled(){}
    virtual void Tick(float deltaTime) {}
    virtual void OnDisabled(){}

    virtual void Render(const ImRect& clipScreen) {}

    virtual bool HandleInputEvent(InputEvent &event) { return false;}
};