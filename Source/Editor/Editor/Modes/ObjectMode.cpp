#include "ObjectMode.h"
#include <Editor/Editor.h>
#include <World/RenderLayer/WorldRenderLayer.h>
#include <RenderV/Engine.h>
#include <Core/Application/Application.h>
#include <World/Utility.h>
#include <Math/Math.h>

void ObjectMode::DrawCircleControl(ImVec2 center, float radius, glm::vec2 cursor_pos, ImGuiMouseCursor cursor_type)
{
    ImGui::GetWindowDrawList()->AddCircleFilled(center, radius, ImColor(0.5f, 0.5f, 1.0f, 1.0f));
    if(MathUtility::BoxPointCollide(glm::vec4(center.x - radius, center.y - radius, 
                                              center.x + radius, center.y + radius),
                                    cursor_pos)
        && ImGui::IsWindowFocused())
    {
        ImGui::SetMouseCursor( cursor_type );
    }
}

void ObjectMode::Render(const ImRect &clipScreen)
{
    if(Entity* ent = dynamic_cast<Entity*>(Editor::Get()->GetSelectedObject())) {
        IVec4 SelectionWorldBounds ={  ent->GetLocation().x, ent->GetLocation().y, 
                                        ent->GetLocation().x + ent->GetBoundingBox().x, ent->GetLocation().y + ent->GetBoundingBox().y}; 

        Vec2 CameraPosition = {static_cast<float>(ent->GetWorld()->GetSceneView().ViewOrigin.x), static_cast<float>(ent->GetWorld()->GetSceneView().ViewOrigin.y)};

        glm::vec2 CursorPos = glm::vec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);

        Vec4 ObjectBounds = {ent->GetLocation().x, 
                                ent->GetLocation().y, 
                                ent->GetLocation().x + (ent->GetBoundingBox().x), 
                                ent->GetLocation().y + (ent->GetBoundingBox().y)};

        const IRenderComposition* comp = GApplication->GetRender()->GetComposition(0);
        VkRect2D ViewportLocation = comp->GetGameViewport();

        glm::vec2 SelectionBoundsMin = WorldUtility::GetScreenSpaceFromWorldSpace(GEngine->GetWorld(), ViewportLocation.offset.x, ViewportLocation.offset.y, ViewportLocation.extent.width, ViewportLocation.extent.height, ObjectBounds.x, ObjectBounds.y);
        glm::vec2 SelectionBoundsMax = WorldUtility::GetScreenSpaceFromWorldSpace(GEngine->GetWorld(), ViewportLocation.offset.x, ViewportLocation.offset.y, ViewportLocation.extent.width, ViewportLocation.extent.height, ObjectBounds.z, ObjectBounds.w);

        // Draw Selection bounds
        ImGui::GetWindowDrawList()->AddRect(ImVec2(clipScreen.Min.x + SelectionBoundsMin.x, 
                                                   clipScreen.Min.y + SelectionBoundsMin.y), 
                                            ImVec2(clipScreen.Min.x + SelectionBoundsMax.x, 
                                                   clipScreen.Min.y + SelectionBoundsMax.y),
                                            ImColor(1.0f, 1.0f, 1.0f, 1.0f));

        // Draw Gizmos
        {
            // Draw Gizmo Corner Controls
            const float ControlCircleRadius = 4.f;
            ImVec2 ControlCenter; 
            // TL
            ControlCenter = ImVec2( clipScreen.Min.x + SelectionBoundsMin.x, 
                                    clipScreen.Min.y + SelectionBoundsMin.y);
            DrawCircleControl(ControlCenter, ControlCircleRadius, CursorPos, ImGuiMouseCursor_ResizeNESW);
            // TR
            ControlCenter = ImVec2( clipScreen.Min.x + SelectionBoundsMax.x, 
                                    clipScreen.Min.y + SelectionBoundsMin.y);
            DrawCircleControl(ControlCenter, ControlCircleRadius, CursorPos, ImGuiMouseCursor_ResizeNWSE);
            // BL
            ControlCenter = ImVec2( clipScreen.Min.x + SelectionBoundsMin.x, 
                                    clipScreen.Min.y + SelectionBoundsMax.y);
            DrawCircleControl(ControlCenter, ControlCircleRadius, CursorPos, ImGuiMouseCursor_ResizeNWSE);
            // BR
            ControlCenter = ImVec2( clipScreen.Min.x + SelectionBoundsMax.x, 
                                    clipScreen.Min.y + SelectionBoundsMax.y);
            DrawCircleControl(ControlCenter, ControlCircleRadius, CursorPos, ImGuiMouseCursor_ResizeNESW);
        }
    }
}

bool ObjectMode::HandleInputEvent(InputEvent &event)
{ 
    // Try selecting object
    if(event.MouseButton == SDL_BUTTON_LEFT && !event.IsUp) {
        IVec2 MouseLocation = {static_cast<int>(event.MouseX), static_cast<int>(event.MouseY)};
        const IRenderComposition* comp = GApplication->GetRender()->GetComposition(0);
        VkRect2D ViewportLocation = comp->GetGameViewport();
        float MouseX = (MouseLocation.x - ViewportLocation.offset.x);
        float MouseY = (MouseLocation.y - ViewportLocation.offset.y);

        HitscanParams params;
        HitResult result = WorldUtility::HitscanCamToWorld(Editor::Get()->GetWorld(), MouseX, MouseY, ViewportLocation.extent.width, ViewportLocation.extent.height, params);
        if(result.HitEntities.size() > 0)
        {
            LOGF(Log, LogObjectMode, "Selected Entity: 0x%08X", result.HitEntities[0]);
            Editor::Get()->SetSelectedObject(result.HitEntities[0]);
        } else {
            Editor::Get()->SetSelectedObject(GEngine->GetWorld());
        }
    }

    // Delete selected object
    if(event.KeyName == "Delete" && event.IsUp) {
        assert(GEngine->GetWorld() != nullptr);

        if(Entity* entity = dynamic_cast<Entity*>(Editor::Get()->GetSelectedObject())) {
            GEngine->GetWorld()->DestroyEntity(entity);
            Editor::Get()->SetSelectedObject(nullptr);
        }
    }

    return false;
}