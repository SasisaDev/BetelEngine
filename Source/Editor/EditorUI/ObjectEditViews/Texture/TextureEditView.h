#pragma once

#include "../ObjectEditViewsFactory.h"

#include <Toolkit/ToolkitWindow.h>
#include <GameFramework/Assets/Texture/Texture.h>

#include <imgui/backends/imgui_impl_vulkan.h>

namespace BImGui
{
    extern bool InputString(const char *id, std::string &string, ImGuiInputTextFlags flags = 0);
};

class TextureEditView : public EditorToolkitWindow
{
    ObjTexture *texture = nullptr;
    VkDescriptorSet DS;

    std::string tex_name, tex_path;
public:
    TextureEditView(Object *tex)
    {
        texture = dynamic_cast<ObjTexture *>(tex);

        tex_name = texture->GetName();
        tex_path = texture->GetPath();
        DS = ImGui_ImplVulkan_AddTexture(texture->GetTexture()->GetSampler(), texture->GetTexture()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }

    ~TextureEditView()
    {
        ImGui_ImplVulkan_RemoveTexture(DS);
    }

    virtual void OnGUI(Window *window) override
    {
        assert(texture != nullptr && "Texture Edit View must be provided with a Texture Object to modify!");

        ImGuiStyle& style = ImGui::GetStyle();

        if (ImGui::Begin("Edit Texture", &Visible, ImGuiWindowFlags_NoCollapse))
        {
            BImGui::InputString("Name", tex_name);
            BImGui::InputString("Path", tex_path);
            ImGui::Image((ImTextureID)DS, ImVec2(250, 250));

            // Draw Buttons
            static float buttonWidth1 = ImGui::CalcTextSize("Cancel").x + style.FramePadding.x * 2.f;
            static float buttonWidth2 = ImGui::CalcTextSize("Save").x + style.FramePadding.x * 2.f;
            static float widthNeeded = buttonWidth1 + style.ItemSpacing.x + buttonWidth2;
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);

            if(ImGui::Button("Cancel")) {
                Visible = false;
            }
            ImGui::SameLine();
            if(ImGui::Button("Save")) {
                SaveObject();
            }
        }
        ImGui::End();
    }

    void SaveObject()
    {
        texture->Rename(tex_name);
        texture->SetPath(tex_path);
        texture->Dirty();
        Visible = false;
    }
};