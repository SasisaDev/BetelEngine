#pragma once
#include <i18n/Text.h>

class EditorMode
{
    Text Name;
public:
    EditorMode(std::string name) : Name("Editor", "ToolkitMode", name){}

    Text &GetName() {return Name;}

    virtual void OnEnabled(){}
    virtual void Tick(float deltaTime) {}
    virtual void OnDisabled(){}
};