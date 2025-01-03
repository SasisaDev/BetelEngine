#pragma once

#include "../Mode.h"
#include <Math/Vector.h>
#include <Input/InputEvent.h>

class Entity;

class ObjectMode : public EditorMode {
public:
    ObjectMode() : EditorMode("ObjectMode") {}

    virtual void OnEnabled() override {}
    virtual void Tick(float deltaTime) override {}
    virtual void OnDisabled() override {}

    virtual bool HandleInputEvent(InputEvent &event) override;
};