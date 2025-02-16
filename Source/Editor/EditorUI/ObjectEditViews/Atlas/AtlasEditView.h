#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <GameFramework/Assets/Atlas/Atlas.h>

#include <imgui/backends/imgui_impl_vulkan.h>
#include <EditorUI/ObjectEditViews/ObjectEditView.h>

#include <ImGui/Betel/BetelImages.h>
#include <ImGui/Betel/BetelDeferred.h>
#include <ImGui/Betel/BetelInputs.h>

#include <optional>

namespace BImGui
{
    extern bool ImageButton(const char *ID, ImTextureID ImgID, ImVec2 ImgSize, ImVec2 Uv0, ImVec2 Uv1);
};

class AtlasEditView : public ObjectEditView
{
    const char *textureName = "Texture";
    const char *rectName = "Bounds";

    struct AtlasResourcesDeleter : public BImGui::DeferredTask
    {
        bool ShouldCleanup = false;
        bool ShouldSelfDestruct = false;

        bool Perform() override
        {
            if (!ShouldCleanup)
            {
                return ShouldSelfDestruct;
            }

            // Cleanup Procedure

            ShouldCleanup = false;

            return ShouldSelfDestruct;
        }
    };

    struct AtlasSaver : public BImGui::DeferredTask
    {
        ObjAtlas *atlas;

        // Local variables copy
        std::string name;
        uint32_t texID = 0;

        bool ShouldSave = false;
        bool ShouldSelfDestruct = false;

        bool Perform() override
        {
            if (!ShouldSave)
            {
                return ShouldSelfDestruct;
            }

            //  Procedure
            if (atlas)
            {
                atlas->Rename(name);

                if(texID) atlas->SetTexture(texID);

                atlas->Dirty();
            }

            ShouldSave = false;

            return ShouldSelfDestruct;
        }
    };

private:
    ImTextureID iconBrowse;

    AtlasResourcesDeleter *deferredDeleter;
    AtlasSaver *deferredSaver;

    ObjAtlas *atlas;

    // Local variables copy
    std::string name;

    bool bHasTex = false;
    uint32_t texID = 0;
    ObjTexture *tex = nullptr;
    VkDescriptorSet texDS = VK_NULL_HANDLE;
    double texAspect = 1;

    std::map<uint16_t, IVec4> sprites;

private:
    void OnTextureObjectSelected()
    {
    }

public:
    AtlasEditView(Object *obj)
        : ObjectEditView("Edit Atlas")
    {
        bResizable = true;

        iconBrowse = BImGui::GetEdImage(BImGui::Img::Browse32Icon);

        deferredDeleter = BImGui::CreateDeferredTask<AtlasResourcesDeleter>();
        deferredSaver = BImGui::CreateDeferredTask<AtlasSaver>();

        atlas = dynamic_cast<ObjAtlas *>(obj);
        name = atlas->GetName();
        tex = atlas->GetTexture().Load();
        if (tex)
        {
            bHasTex = true;
            texID = tex->GetID();
            texDS = ImGui_ImplVulkan_AddTexture(tex->GetTexture()->GetSampler(), tex->GetTexture()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            texAspect = static_cast<double>(tex->GetWidth()) / static_cast<double>(tex->GetHeight());
        }
        sprites = atlas->GetSpriteRects();
    }

    ~AtlasEditView()
    {
        deferredDeleter->ShouldCleanup = true;
        deferredDeleter->ShouldSelfDestruct = true;
        deferredSaver->ShouldSelfDestruct = true;
    }

    virtual float GetCustomControlButtonsWidth(const ImGuiStyle &style) override
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
        std::string inpTexObjFilter;
        BImGui::InputObject("Texture", texID, inpTexObjFilter, "TEX");

        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDocking;
        ImGuiViewport *viewport = ImGui::GetWindowViewport();

        ImVec2 ActualViewport = ImVec2(viewport->Size.x, 
                                        viewport->Size.y
                                        + (ImGui::CalcTextSize("Text").y + ImGui::GetStyle().FramePadding.y * 2.f));

        ImGuiID dockspace_id = ImGui::GetID("AtlasEditViewDockSpace");
        ImGui::SetNextWindowBgAlpha(.0f);
        ImGui::DockSpace(dockspace_id, ImVec2(0, 0), dockspace_flags, nullptr);

        static auto first_time = true;
        if (first_time)
        {
            first_time = false;

            ImGui::DockBuilderRemoveNode(dockspace_id);
            ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspace_id, ActualViewport);

            auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.2f, nullptr, &dockspace_id);

            // we now dock our windows into the docking node we made above
            ImGui::DockBuilderDockWindow(textureName, dockspace_id);
            ImGui::DockBuilderDockWindow(rectName, dock_id_right);
            ImGui::DockBuilderFinish(dockspace_id);
        }

        ImGuiWindowClass noTab_class;
        //noTab_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

        ImGui::SetNextWindowClass(&noTab_class);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6, 6));
        if (ImGui::Begin(textureName, 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
        {
            if (bHasTex)
            {
                ImGui::Image((ImTextureID)texDS, ImVec2(250, texAspect * 250));
            }
        }
        ImGui::PopStyleVar();
        ImGui::End();

        ImGui::SetNextWindowClass(&noTab_class);
        if (ImGui::Begin(rectName, 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
        {
            
        }
        ImGui::End();
    }

    virtual bool SaveObject() override
    {
        if (name != atlas->GetName() || texID != atlas->GetTexture().GetID())
        {
            deferredSaver->atlas = atlas;
            deferredSaver->name = name;
            deferredSaver->texID = texID;
            deferredSaver->ShouldSave = true;
        }

        return true;
    }
};