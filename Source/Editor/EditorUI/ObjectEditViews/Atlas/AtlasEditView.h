#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <GameFramework/Assets/Atlas/Atlas.h>

#include <imgui/backends/imgui_impl_vulkan.h>
#include <EditorUI/ObjectEditViews/ObjectEditView.h>

#include <ImGui/Betel/BetelImages.h>
#include <ImGui/Betel/BetelDeferred.h>
#include <ImGui/Betel/BetelInputs.h>

#include <optional>
#include <algorithm>

#pragma clang diagnostic ignored "-Wdeprecated-enum-enum-conversion"// warning: bitwise operation between different enumeration types ('XXXFlags_' and 'XXXFlagsPrivate_') is deprecated

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
    std::map<uint16_t, ImU32> spriteRectsColors;

    // Texture Editor variables
    float TexEd_Zoom = 1;
    ImVec2 TexEd_Offset = {};
    bool TexEd_Drag = false;
    ImVec2 TexEd_DragStartPosition = {};
    ImVec2 TexEd_DragOffset = {};

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

    void DrawTextureEditor()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6, 6));
        if (ImGui::Begin(textureName, 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
        {
            ImRect WindowSpace = ImGui::GetCurrentWindow()->WorkRect;

            if (!bHasTex)
            {
                ImGui::PopStyleVar();
                ImGui::End();
                return;
            }

            // Handle input

            if(ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) 
            {
                TexEd_Drag = true;
                TexEd_DragStartPosition = ImGui::GetIO().MousePos;
            } 
            else if(ImGui::IsMouseReleased(ImGuiMouseButton_Middle))
            {
                TexEd_Drag = false;
                TexEd_Offset = ImVec2(TexEd_Offset.x - TexEd_DragOffset.x, TexEd_Offset.y - TexEd_DragOffset.y);
                TexEd_DragOffset = ImVec2(0, 0);
            }

            if(TexEd_Drag)
            {
                TexEd_DragOffset = ImVec2(TexEd_DragStartPosition.x - ImGui::GetIO().MousePos.x, TexEd_DragStartPosition.y - ImGui::GetIO().MousePos.y);
            }

            TexEd_Zoom = std::clamp(TexEd_Zoom + ImGui::GetIO().MouseWheel / (15.f / TexEd_Zoom), 0.05f, 20.f);

            ImColor GridCol(1.f, 1.f, 1.f, 0.25f);
            for(int gRow = 0; gRow < std::ceil<int>(WindowSpace.GetHeight() * TexEd_Zoom); ++gRow)
            {
                ImGui::GetWindowDrawList()->AddLine(ImVec2( WindowSpace.Min.x, 
                                                            WindowSpace.Min.y + TexEd_Offset.y - TexEd_DragOffset.y + (gRow * TexEd_Zoom)),
                                                    ImVec2( WindowSpace.Max.x, 
                                                            WindowSpace.Min.y + TexEd_Offset.y - TexEd_DragOffset.y + (gRow * TexEd_Zoom)),
                                                    GridCol);
            }
            for(int gCol = 0; gCol < std::ceil<int>(WindowSpace.GetWidth() * TexEd_Zoom); ++gCol)
            {
                ImGui::GetWindowDrawList()->AddLine(ImVec2( WindowSpace.Min.x + TexEd_Offset.x - TexEd_DragOffset.x + (gCol * TexEd_Zoom), 
                                                            WindowSpace.Min.y),
                                                    ImVec2( WindowSpace.Min.x + TexEd_Offset.x - TexEd_DragOffset.x + (gCol * TexEd_Zoom), 
                                                            WindowSpace.Max.y),
                                                    GridCol);
            }

            ImGui::GetWindowDrawList()->AddImage((ImTextureID)texDS, 
                                                 ImVec2(WindowSpace.Min.x + TexEd_Offset.x - TexEd_DragOffset.x, WindowSpace.Min.y + TexEd_Offset.y - TexEd_DragOffset.y), 
                                                 ImVec2(WindowSpace.Min.x + TexEd_Offset.x - TexEd_DragOffset.x + (tex->GetWidth() * TexEd_Zoom), WindowSpace.Min.y + TexEd_Offset.y - TexEd_DragOffset.y + (tex->GetHeight() * TexEd_Zoom)));

            // Draw image boundaries
            ImGui::GetWindowDrawList()->AddRect(ImVec2(WindowSpace.Min.x + TexEd_Offset.x - TexEd_DragOffset.x, WindowSpace.Min.y + TexEd_Offset.y - TexEd_DragOffset.y), 
                                                ImVec2(WindowSpace.Min.x + TexEd_Offset.x - TexEd_DragOffset.x + (tex->GetWidth() * TexEd_Zoom), WindowSpace.Min.y + TexEd_Offset.y - TexEd_DragOffset.y + (tex->GetHeight() * TexEd_Zoom)),
                                                -1);
        }
        ImGui::PopStyleVar();
        ImGui::End();
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

        DrawTextureEditor();

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