#include "DebugImGui.h"

#include <Engine/Engine.h>
#include <World/World.h>

#include <array>

#define FPS_LIST_SIZE 32

void DebugImGui::DrawFPSCounter()
{
    static std::array<float, FPS_LIST_SIZE> fpsList;
    static size_t fpsListIdx = 0;
    if (fpsListIdx >= FPS_LIST_SIZE)
        fpsListIdx = 0;

    fpsList[fpsListIdx] = 1.0f / GEngine->GetDeltaTime();
    ++fpsListIdx;

    float fpsSum = 0;
    for (float fpsF : fpsList)
    {
        fpsSum += fpsF;
    }

    uint32_t fps = static_cast<uint32_t>(fpsSum / FPS_LIST_SIZE);

    ImGui::Text("FPS: %u", fps);
}

void DebugImGui::OnGUI(Window *window)
{
    if (!bRender)
        return;

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.925f));
    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Once);
    if (ImGui::Begin("Debug Menu"))
    {
        DrawFPSCounter();

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
    ImGui::PopStyleColor();
}