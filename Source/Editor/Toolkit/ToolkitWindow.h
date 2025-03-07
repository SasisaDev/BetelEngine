#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <i18n/Text.h>

class Window;

class EditorToolkitWindow
{
public:
    bool Visible = true;

    virtual ~EditorToolkitWindow(){}

    virtual void DrawGUI(Window* window){if(Visible){this->OnGUI(window);}}
    virtual const char* GetName(){return "";}
    // Should not be called by toolkits
    virtual void OnGUI(Window* window){}
};