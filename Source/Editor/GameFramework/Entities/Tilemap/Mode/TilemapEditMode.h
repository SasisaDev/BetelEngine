#pragma once

#include <Editor/Modes/Mode.h>

#include <Math/Vector.h>
#include <Input/InputEvent.h>

class Entity;

class TilemapEditMode : public EditorMode {
public:
    TilemapEditMode() : EditorMode("TilemapEditMode") {}

    virtual void OnEnabled() override;
    virtual void Tick(float deltaTime) override;
    virtual void OnDisabled() override;

    virtual bool HandleInputEvent(InputEvent &event) override;
};