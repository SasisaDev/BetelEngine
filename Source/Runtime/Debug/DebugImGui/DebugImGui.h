#pragma once

#include <imgui/imgui.h>

class DebugImGui
{
    static bool bRender;
public:
    void Toggle() {bRender = !bRender;}
    void Draw() {
        
    }
};

inline bool DebugImGui::bRender = false;