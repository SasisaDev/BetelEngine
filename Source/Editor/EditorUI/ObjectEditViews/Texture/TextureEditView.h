#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <GameFramework/Assets/Texture/Texture.h>

#include <imgui/backends/imgui_impl_vulkan.h>
#include <EditorUI/ObjectEditViews/ObjectEditView.h>

#include <EditorUI/WindowLibrary/BetelImages.h>
#include <EditorUI/WindowLibrary/BetelDeferredCleanup.h>

#include <optional>

namespace BImGui
{
    extern bool InputString(const char *id, std::string &string, ImGuiInputTextFlags flags = 0);
    extern bool ImageButton(const char* ID, ImTextureID ImgID, ImVec2 ImgSize, ImVec2 Uv0 = ImVec2(0, 0), ImVec2 Uv1 = ImVec2(1, 1));
};

class TextureEditView : public ObjectEditView
{
    struct TextureResourcesDeleter : public BImGui::DeferredDeleter
    {
        std::optional<EditorTextureData> tex_reimport;
        VkDescriptorSet tex_ds;
        bool ShouldCleanup = false;
        bool ShouldSelfDestruct = false;

        bool Cleanup() override {
            if(!ShouldCleanup) {
                return ShouldSelfDestruct;
            }

            if(tex_reimport.has_value())
            {
                // Cleanup previous reimport
                EditorImageLoader::Get().FreeTexture(tex_reimport.value());
                tex_reimport.reset();
            } else {
                // We didn't reimport yet, so tex_ds is constructed from Texture file
                ImGui_ImplVulkan_RemoveTexture(tex_ds);
            }

            ShouldCleanup = false;

            return ShouldSelfDestruct;
        }
    };

private:

    ImTextureID icon_browse;

    bool bReimport = false;
    bool bReimportFailed = false;

    TextureResourcesDeleter *deferredDeleter;

    ObjTexture *texture = nullptr;
    std::optional<EditorTextureData> tex_reimport;
    VkDescriptorSet tex_ds;
    std::string tex_name, tex_path, tex_dimensions;
    double tex_aspect;
private:
    // FIXME: This causes memory leak
    void Reimport() {
        EditorTextureData data;
        const std::string path = (IPlatform::Get()->GetExecutableFolder() + "/Content/" + tex_path);
        EditorImageLoader::Get().LoadTextureFromFile(path.c_str(), &data, true);
        // Check if loading failed
        if(data.DS == VK_NULL_HANDLE) {
            bReimportFailed = true;
            return;
        }
        bReimportFailed = false;

        // Setup deferred deleter
        deferredDeleter->tex_reimport = tex_reimport;
        deferredDeleter->tex_ds = tex_ds;
        deferredDeleter->ShouldCleanup = true;

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

        deferredDeleter = BImGui::CreateDeferredDeleter<TextureResourcesDeleter>();
    }

    ~TextureEditView()
    {
        // FIXME: As ImGui render happens in render pass and object deletion happens on game pass(before render pass)
        // At RenderTime DescriptorSet is already deleted, which causes Validation Layer's errors
        
        // Setup deferred deleter
        deferredDeleter->tex_reimport = tex_reimport;
        deferredDeleter->tex_ds = tex_ds;
        deferredDeleter->ShouldCleanup = true;
        deferredDeleter->ShouldSelfDestruct = true;
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

        if(bReimportFailed) {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, BImGui::Colors::ErrorRedColor);
        }
        BImGui::InputString("##Path", tex_path);
        if(bReimportFailed) {
            ImGui::PopStyleColor();
        }
        ImGui::SameLine();
        BImGui::ImageButton("##PickImagePath", icon_browse, ImVec2(13, 13));
        ImGui::SameLine();
        ImGui::Text("Path");

        ImGui::Text("Dimensions: ");
        ImGui::SameLine();
        ImGui::Text("%s", tex_dimensions.c_str());

        ImGui::Image((ImTextureID)tex_ds, ImVec2(250, tex_aspect * 250));
    }

    virtual void SaveObject() override
    {
        if(tex_name != texture->GetName() || tex_path != texture->GetPath())
        {
            texture->Rename(tex_name);
            texture->SetPath(tex_path);
            texture->LoadTexture();
            texture->Dirty();
        }
    }
};