#pragma once

#include "../Mode.h"

class Entity;

class ObjectMode : public EditorMode {
    Entity* SelectedEntity;
public:
    ObjectMode() : EditorMode("ObjectMode") {}

    virtual void OnEnabled() override {}
    virtual void Tick(float deltaTime) override {}
    virtual void OnDisabled() override {}
};