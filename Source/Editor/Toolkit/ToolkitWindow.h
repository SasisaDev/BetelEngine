#pragma once

#include <imgui.h>
#include <imgui_internal.h>

#include <i18n/Text.h>

class Window;

class EditorToolkitWindow
{
public:
    bool Visible = true;

    virtual void DrawGUI(Window* window){if(Visible){this->OnGUI(window);}}

    // Should not be called by toolkits
    virtual void OnGUI(Window* window){}
};