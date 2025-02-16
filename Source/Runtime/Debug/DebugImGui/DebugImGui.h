#pragma once

#include <imgui/imgui.h>
#include "../../../Editor/Toolkit/Toolkit.h"

class DebugImGui : public EditorToolkit 
{
    bool bRender = false;
public:
    static DebugImGui* Get() {
        static DebugImGui* debugImGui = new DebugImGui;
        return debugImGui;
    }
    void Toggle() {bRender = !bRender;}
    virtual void OnGUI(Window* window) override 
    {

    }
};