#pragma once

#include <Editor/Mode.h>

class SpriteDrawMode : public EditorMode {
public:
    SpriteDrawMode() : EditorMode("Editor.ToolkitMode.SpriteDrawMode") {}

    virtual void OnEnabled() override {}
    virtual void Tick(float deltaTime) override {}
    virtual void OnDisabled() override {}

    virtual void Render(const ImRect& clipScreen) override;

    virtual bool HandleInputEvent(InputEvent &event) override;
};