#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <GameFramework/Assets/Texture/Texture.h>

#include <imgui/backends/imgui_impl_vulkan.h>
#include <EditorUI/ObjectEditViews/ObjectEditView.h>

#include <EditorUI/WindowLibrary/BetelImages.h>

#include <optional>

namespace BImGui
{
    extern bool InputString(const char *id, std::string &string, ImGuiInputTextFlags flags = 0);
    extern bool ImageButton(const char* ID, ImTextureID ImgID, ImVec2 ImgSize, ImVec2 Uv0 = ImVec2(0, 0), ImVec2 Uv1 = ImVec2(1, 1));
};

class TextureEditView : public ObjectEditView
{
    ImTextureID icon_browse;

    bool bReimport = false;

    ObjTexture *texture = nullptr;
    std::optional<EditorTextureData> tex_reimport;
    VkDescriptorSet tex_ds;
    std::string tex_name, tex_path, tex_dimensions;
    double tex_aspect;
private:
    // FIXME: This causes memory leak
    void Reimport() {
        if(tex_reimport.has_value())
        {
            // Cleanup
            EditorImageLoader::Get().FreeTexture(tex_reimport.value());
            //ImGui_ImplVulkan_RemoveTexture(tex_ds);
            tex_reimport.reset();
        } else {
            // We didn't reimport yet, so tex_ds is constructed from Texture file
            ImGui_ImplVulkan_RemoveTexture(tex_ds);
        }
        EditorTextureData data;
        const std::string path = (IPlatform::Get()->GetExecutableFolder() + "/Content/" + tex_path);
        EditorImageLoader::Get().LoadTextureFromFile(path.c_str(), &data, true);
        tex_reimport = data;
        tex_ds = tex_reimport.value().DS;
        tex_dimensions = std::to_string(data.Width) + " x " + std::to_string(data.Height);
        tex_aspect = static_cast<double>(data.Width) / static_cast<double>(data.Height);
    }

public:
    TextureEditView(Object *tex)
        : ObjectEditView("Edit Texture")
    {
        icon_browse = BImGui::GetEdImage(BImGui::Img::Browse32Icon);

        texture = dynamic_cast<ObjTexture *>(tex);

        tex_name = texture->GetName();
        tex_path = texture->GetPath();
        tex_dimensions = std::to_string(texture->GetWidth()) + " x " + std::to_string(texture->GetHeight());
        tex_aspect = static_cast<double>(texture->GetHeight()) / static_cast<double>(texture->GetWidth());
        tex_ds = ImGui_ImplVulkan_AddTexture(texture->GetTexture()->GetSampler(), texture->GetTexture()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }

    ~TextureEditView()
    {
        ImGui_ImplVulkan_RemoveTexture(tex_ds);
    }

    virtual float GetCustomControlButtonsWidth(const ImGuiStyle& style) override
    {
        return style.ItemSpacing.x + ImGui::CalcTextSize("Reimport").x + style.FramePadding.x * 2.f;
    }

    virtual void DrawCustomControlButtons() override
    {
        if(ImGui::Button("Reimport"))
        {
            bReimport = true;
        }
        ImGui::SameLine();
    }

    virtual void OnEditViewGUI(Window *window) override
    {
        assert(texture != nullptr && "Texture Edit View must be provided with a Texture Object to modify!");

        if(bReimport) {
            Reimport();
            bReimport = false;
        }

        BImGui::InputString("Name", tex_name);

        BImGui::InputString("##Path", tex_path);
        ImGui::SameLine();
        BImGui::ImageButton("##PickImagePath", icon_browse, ImVec2(13, 13));
        ImGui::SameLine();
        ImGui::Text("Path", tex_path);

        ImGui::Text("Dimensions: ");
        ImGui::SameLine();
        ImGui::Text(tex_dimensions.c_str());

        ImGui::Image((ImTextureID)tex_ds, ImVec2(250, tex_aspect * 250));
    }

    virtual void SaveObject() override
    {
        texture->Rename(tex_name);
        texture->SetPath(tex_path);
        texture->LoadTexture();
        texture->Dirty();
    }
};