#pragma once
#include <imgui/imgui_internal.h>

namespace BImGui {
    namespace Colors {
        static ImVec4 UnselectedColor = ImVec4(0.15, 0.15, 0.15, 1);
        static ImVec4 UnselectedToggleColor = ImVec4(0, 0, 0, 0);
        static ImVec4 ContrastColor = ImVec4(0, 0.647, 1, 1);
        static ImVec4 ErrorRedColor = ImVec4(1, 0.0, 0.0, 0.9);

        extern ImVec4 RandomColor(unsigned int seed);
        extern ImVec4 RandomBrightColor(unsigned int seed);

        extern float GetBrightness(ImVec4 color);
    };
};