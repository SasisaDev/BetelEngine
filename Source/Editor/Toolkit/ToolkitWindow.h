#pragma once

#include <imgui.h>

class EditorToolkitWindow
{
public:
    bool Visible = true;

    virtual void DrawGUI(){if(Visible){this->OnGUI();}}

    // Should not be called by toolkits
    virtual void OnGUI(){}
};