#include "DebugImGui.h"

#include <Engine/Engine.h>
#include <World/World.h>

void DebugImGui::OnGUI(Window *window)
{
    if(!bRender)
        return;

    //ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always);
    if (ImGui::Begin("Debug Menu"))
    {
        if (GEngine->GetWorld()->GetTimeSpeed() > 0)
        {
            if (ImGui::Button("Stop Time"))
            {
                GEngine->GetWorld()->SetTimeSpeed(0);
            }
        }
        else
        {
            if (ImGui::Button("Resume Time"))
            {
                GEngine->GetWorld()->SetTimeSpeed(1);
            }
        }
    }
    ImGui::End();
}