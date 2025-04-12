#include "SpriteDrawMode.h"

#include <ImGui/Betel/BetelImages.h>

void SpriteDrawMode::RenderToolbar()
{
    if(ImGui::Begin("Toolbar", 0, ImGuiWindowFlags_NoCollapse))
    {
        ImGui::ImageButton("Pencil", BImGui::GetEdImage(BImGui::Img::Pencil32Icon), {16, 16});
        ImGui::SetItemTooltip("%s", "Draw individual pixels");

        ImGui::ImageButton("Eraser", BImGui::GetEdImage(BImGui::Img::Eraser32Icon), {16, 16});
        ImGui::SetItemTooltip("%s", "Erase individual pixels");
    }
    ImGui::End();
}

void SpriteDrawMode::Render(const ImRect &clipScreen)
{
    RenderToolbar();
}

bool SpriteDrawMode::HandleInputEvent(InputEvent &event)
{
    return false;
}
