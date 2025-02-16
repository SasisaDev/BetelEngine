#pragma once

#include "ToolkitWindow.h"
#include <ImGui/ImGuiRenderable.h>

class EditorToolkit : public ImGuiRenderable
{
public:

    virtual void OnGUI(Window* window) override {}
};