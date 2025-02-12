#include "ObjectMode.h"
#include <Editor/Editor.h>
#include <World/RenderLayer/WorldRenderLayer.h>
#include <RenderV/Engine.h>
#include <Core/Application/Application.h>
#include <World/Utility.h>

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
            Editor::Get()->SetSelectedEntity(result.HitEntities[0]);
        } else {
            Editor::Get()->SetSelectedEntity(nullptr);
        }
    }

    // Delete selected object
    if(event.KeyName == "Delete" && event.IsUp) {
        assert(GEngine->GetWorld() != nullptr);

        if(Entity* entity = Editor::Get()->GetSelectedEntity()) {
            GEngine->GetWorld()->DestroyEntity(entity);
            Editor::Get()->SetSelectedEntity(nullptr);
        }
    }

    return false;
}