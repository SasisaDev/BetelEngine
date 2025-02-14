#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <Object/ObjectTypeLibrary.h>
#include <World/Utility.h>
#include <Core/Application/Application.h>
#include <Editor/Editor.h>
#include "../../EditorImageLoader.h"
#include <World/RenderLayer/WorldRenderLayer.h>
#include <EditorUI/WindowLibrary/BetelImages.h>
#include <EditorUI/WindowLibrary/BetelDrag.h>

#include <cassert>

class EditorViewport : public EditorToolkitWindow
{
    ImTextureID ShowOverlayImageW;

    int Width = 0, Height = 0;

public:
    const char *GetName() override { return "Game"; }

    virtual void DrawGUI(Window *window) override
    {
        if (Visible)
        {
            this->OnGUI(window);
        }
        else
        {
            IRenderComposition *comp = GApplication->GetRender()->GetComposition(window->GetCompositionID());
            comp->SetGameViewport({0});
            if (GApplication && GApplication->GetEngine())
                GApplication->GetEngine()->SetGameFocused(false);
        }
    }

    void HandleDragAndDrop(uint32_t dragObjectID, ImVec4 dragPosition = ImVec4(0,0,0,0))
    {
        if (World *world = GEngine->GetWorld())
        {
            Object *object = GEngine->GetObjectLibrary()->LoadObject(dragObjectID);
            assert(object != nullptr);
            ObjectType *objectType = ObjectTypeLibrary::Get().GetObjectType(object->GetType());
            assert(objectType != nullptr);

            if (!objectType->CanSpawnIntoWorld())
            {
                LOG(Warning, LogSpawnOnDrag, "Attempted to drag object into viewport, but it doesn't support this operation");
                return;
            }

            Entity* objectEntity = objectType->CreateWorldEntity(object);
            assert(objectEntity != nullptr);

            EntitySpawnInfo spawnInfo;
            spawnInfo.Location = IVec3(WorldUtility::GetWorldSpaceFromScreenSpace(world, dragPosition.x, dragPosition.y, dragPosition.z, dragPosition.w), 0);
            world->AddEntity(objectEntity, spawnInfo);
        }
    }

    virtual void OnGUI(Window *window)
    {
        static bool first_init = true;
        if (first_init)
        {
            ShowOverlayImageW = BImGui::GetEdImage(BImGui::Img::Visibility32Icon);
            ImVec2 availableSize = ImGui::GetContentRegionAvail();
            Width = availableSize.x;
            Height = availableSize.y;

            first_init = false;
        }

        IRenderComposition *comp = GApplication->GetRender()->GetComposition(window->GetCompositionID());

        // Constant values
        static const int32_t WindowPadding = 5;
        static const int WorldLayerID = comp->GetLayerIDFromType<WorldRenderLayerRef>();

        // Fix docked window background being different from the undocked
        static ImGuiWindowClass vpClass;
        vpClass.ClassId = ImGui::GetID("GameViewport");
        vpClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_HiddenTabBar;

        ImGui::SetNextWindowClass(&vpClass);
        // ImGui::SetNextWindowBgAlpha(1.f);
        ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Once);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(WindowPadding, WindowPadding));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
        if (ImGui::Begin(GetName(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar))
        {
            if (ImGui::IsWindowDocked())
            {
                ImGuiID dockID = ImGui::GetWindowDockID();
            }

            // TODO: Handle window resize in editor
            ImVec2 availableSize = ImGui::GetContentRegionAvail();
            WorldRenderLayerRef *layer = dynamic_cast<WorldRenderLayerRef *>(comp->GetLayer(WorldLayerID));

            layer->SetViewportSize({static_cast<unsigned int>(availableSize.x), static_cast<unsigned int>(availableSize.y)});

            // Set game viewport
            ImGuiWindow *window = ImGui::GetCurrentWindowRead();
            ImVec2 viewport = ImGui::GetMainViewport()->Pos;
            ImRect workRect = window->WorkRect;
            comp->SetGameViewport({{(int)workRect.GetTL().x - WindowPadding - (int)viewport.x,
                                    (int)workRect.GetTL().y - WindowPadding - (int)viewport.y},
                                   {(unsigned int)workRect.GetWidth() + WindowPadding * 2,
                                    (unsigned int)workRect.GetHeight() + WindowPadding * 2}});

            // In editor mod set, if the game is focused or not
            if (GApplication && GApplication->GetEngine())
                GApplication->GetEngine()->SetGameFocused(ImGui::IsWindowFocused());

            // Draw Toolbar Elements
            // TODO: Editor Mod Fetch & Registration
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15, 0.15, 0.15, 0.4));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15, 0.15, 0.15, 0.5));
            ImGui::SetNextItemWidth(175);
            static std::vector<const char *> modeNames;
            modeNames.clear();
            for (EditorMode *mode : Editor::Get()->Modes)
            {
                modeNames.push_back(mode->GetName().Get().c_str());
            }
            ImGui::Combo("##EditorMode", &Editor::Get()->CurrentActiveMode, modeNames.data(), IM_ARRAYSIZE(modeNames.data()));
            ImGui::PopStyleColor(2);

            // Show Editor Overlay Button
            ImGui::SameLine();
            if (Editor::Get()->bShowOverlay)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.647, 1, 0.9));
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.6));
            }
            if (ImGui::ImageButton("##ShowOverlay", ShowOverlayImageW, ImVec2(14, 14)))
            {
                Editor::Get()->bShowOverlay = !Editor::Get()->bShowOverlay;
            }
            ImGui::PopStyleColor(1);

            // Viewport Hovered setup
            Editor::Get()->SetViewportHovered(ImGui::IsWindowHovered());

            // TODO: Drag functionality
            // Get an ID of an object currently being dragged
            uint32_t dragObjectID = BImGui::Drag::GetObject();
            if (ImGui::IsWindowHovered() && dragObjectID != 0)
            {
                ImVec2 RawMouseLocation = ImGui::GetMousePos();
                IVec2 MouseLocation = {static_cast<int>(RawMouseLocation.x - viewport.x), 
                                       static_cast<int>(RawMouseLocation.y - viewport.y)};
                const IRenderComposition* comp = GApplication->GetRender()->GetComposition(0);
                VkRect2D ViewportLocation = comp->GetGameViewport();
                float MouseX = (MouseLocation.x - ViewportLocation.offset.x);
                float MouseY = (MouseLocation.y - ViewportLocation.offset.y);
                HandleDragAndDrop(dragObjectID, ImVec4(MouseX, MouseY, ViewportLocation.extent.width, ViewportLocation.extent.height));
                BImGui::Drag::End(dragObjectID);
            }
        }
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(1);
        ImGui::End();
    }
};