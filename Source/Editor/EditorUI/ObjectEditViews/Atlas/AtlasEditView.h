#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <GameFramework/Assets/Atlas/Atlas.h>

#include <imgui/backends/imgui_impl_vulkan.h>
#include <EditorUI/ObjectEditViews/ObjectEditView.h>

#include <EditorUI/WindowLibrary/BetelImages.h>
#include <EditorUI/WindowLibrary/BetelDeferred.h>

#include <optional>

namespace BImGui
{
    extern bool InputString(const char *id, std::string &string, ImGuiInputTextFlags flags);
    extern bool ImageButton(const char* ID, ImTextureID ImgID, ImVec2 ImgSize, ImVec2 Uv0, ImVec2 Uv1);
};

class AtlasEditView : public ObjectEditView
{
    struct AtlasResourcesDeleter : public BImGui::DeferredTask
    {
        bool ShouldCleanup = false;
        bool ShouldSelfDestruct = false;

        bool Perform() override {
            if(!ShouldCleanup) {
                return ShouldSelfDestruct;
            }

            // Cleanup Procedure

            ShouldCleanup = false;

            return ShouldSelfDestruct;
        }
    };

private:
    ImTextureID iconBrowse;

    AtlasResourcesDeleter *deferredDeleter;
    
    ObjAtlas* atlas;

    // Local variables copy
    std::string name;
    uint32_t texID;
    ObjTexture* tex;
    VkDescriptorSet texDS;
    double texAspect;
    std::map<uint16_t, IVec4> sprites;
public:
    AtlasEditView(Object *obj)
        : ObjectEditView("Edit Atlas")
    {
        iconBrowse = BImGui::GetEdImage(BImGui::Img::Browse32Icon);

        deferredDeleter = BImGui::CreateDeferredTask<AtlasResourcesDeleter>();

        atlas = dynamic_cast<ObjAtlas*>(obj);
        name = atlas->GetName();
        tex = atlas->GetTexture().Load();
        texID = tex->GetID();
        texDS = ImGui_ImplVulkan_AddTexture(tex->GetTexture()->GetSampler(), tex->GetTexture()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        texAspect = static_cast<double>(tex->GetWidth()) / static_cast<double>(tex->GetHeight());
        sprites = atlas->GetSpriteRects();
    }

    ~AtlasEditView()
    {
        deferredDeleter->ShouldCleanup = true;
        deferredDeleter->ShouldSelfDestruct = true;
    }

    virtual float GetCustomControlButtonsWidth(const ImGuiStyle& style) override
    {
        return 0;
    }

    virtual void DrawCustomControlButtons() override
    {
    }

    virtual void OnEditViewGUI(Window *window) override
    {
        assert(atlas != nullptr && "Atlas Edit View must be provided with an Atlas Object to modify!");

        BImGui::InputString("Name", name);

        ImGui::Image((ImTextureID)texDS, ImVec2(250, texAspect * 250));
    }

    virtual bool SaveObject() override
    {
        if(name != tex->GetName() || texID != atlas->GetTexture()->GetID())
        {
            atlas->Rename(name);
            atlas->SetTexture(texID);
            atlas->SetSpriteRects(sprites);
            atlas->Dirty();
        }

        return true;
    }
};