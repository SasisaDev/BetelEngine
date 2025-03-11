#include "ObjectMode.h"
#include <Editor/Editor.h>
#include <World/RenderLayer/WorldRenderLayer.h>
#include <RenderV/Engine.h>
#include <Core/Application/Application.h>
#include <World/Utility.h>

void ObjectMode::Render(const ImRect& clipScreen)
{
    if(Entity* ent = dynamic_cast<Entity*>(Editor::Get()->GetSelectedObject())) {
        IVec4 SelectionWorldBounds ={  ent->GetLocation().x, ent->GetLocation().y, 
                                        ent->GetLocation().x + ent->GetBoundingBox().x, ent->GetLocation().y + ent->GetBoundingBox().y}; 

        Vec2 CameraPosition = {static_cast<float>(ent->GetWorld()->GetWorldCameraPosition().x), static_cast<float>(ent->GetWorld()->GetWorldCameraPosition().y)};

        // TODO: Remove Viewport Zoom multiplication
        Vec4 ObjectBounds = {ent->GetLocation().x - CameraPosition.x, 
                                ent->GetLocation().y + CameraPosition.y, 
                                ent->GetLocation().x + (ent->GetBoundingBox().x * Editor::Get()->ViewportZoom) - CameraPosition.x, 
                                ent->GetLocation().y + (ent->GetBoundingBox().y * Editor::Get()->ViewportZoom) + CameraPosition.y};

        IVec2 SelectionBoundsMin = WorldUtility::GetScreenSpaceFromWorldSpace(GEngine->GetWorld(), 0, 0, 0, 0);
        IVec2 SelectionBoundsMax = WorldUtility::GetScreenSpaceFromWorldSpace(GEngine->GetWorld(), 0, 0, 0, 0);

        // Test purposes
        SelectionBoundsMin = {ObjectBounds.x, ObjectBounds.y};
        SelectionBoundsMax = {ObjectBounds.z, ObjectBounds.w};

        // Draw Selection bounds
        ImGui::GetWindowDrawList()->AddRect(ImVec2(clipScreen.Min.x + static_cast<float>(SelectionBoundsMin.x), 
                                                   clipScreen.Min.y + static_cast<float>(SelectionBoundsMin.y)), 
                                            ImVec2(clipScreen.Min.x + static_cast<float>(SelectionBoundsMax.x), 
                                                   clipScreen.Min.y + static_cast<float>(SelectionBoundsMax.y)),
                                            ImColor(1.0f, 1.0f, 1.0f, 1.0f));

        // Draw Gizmos
        {
            // Draw Gizmo Corner Controls
            const float ControlCircleRadius = 4.f;
            // TL
            ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2( clipScreen.Min.x + static_cast<float>(SelectionBoundsMin.x), 
                                                                clipScreen.Min.y + static_cast<float>(SelectionBoundsMin.y)),
                                                                ControlCircleRadius, ImColor(0.5f, 0.5f, 1.0f, 1.0f));
            // TR
            ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2( clipScreen.Min.x + static_cast<float>(SelectionBoundsMax.x), 
                                                                clipScreen.Min.y + static_cast<float>(SelectionBoundsMin.y)),
                                                                ControlCircleRadius, ImColor(0.5f, 0.5f, 1.0f, 1.0f));
            // BL
            ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2( clipScreen.Min.x + static_cast<float>(SelectionBoundsMin.x), 
                                                                clipScreen.Min.y + static_cast<float>(SelectionBoundsMax.y)),
                                                                ControlCircleRadius, ImColor(0.5f, 0.5f, 1.0f, 1.0f));
            // BR
            ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2( clipScreen.Min.x + static_cast<float>(SelectionBoundsMax.x), 
                                                                clipScreen.Min.y + static_cast<float>(SelectionBoundsMax.y)),
                                                                ControlCircleRadius, ImColor(0.5f, 0.5f, 1.0f, 1.0f));
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