#pragma once

#include <imgui.h>

class Window;

class EditorToolkitWindow
{
public:
    bool Visible = true;

    virtual void DrawGUI(Window* window){if(Visible){this->OnGUI(window);}}

    // Should not be called by toolkits
    virtual void OnGUI(Window* window){}
};