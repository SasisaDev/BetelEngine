#include "ObjectMode.h"
#include <Editor/Editor.h>
#include <World/RenderLayer/WorldRenderLayer.h>
#include <RenderV/Engine.h>
#include <Core/Application/Application.h>
#include <World/Utility.h>

bool ObjectMode::HandleInputEvent(InputEvent &event)
{ 
    // Try selecting object
    if(event.MouseButton == SDL_BUTTON_LEFT) {
        IVec2 MouseLocation = {static_cast<int>(event.MouseX), static_cast<int>(event.MouseY)};
        const IRenderComposition* comp = GApplication->GetRender()->GetComposition(0);
        VkRect2D ViewportLocation = comp->GetGameViewport();
        float MouseX = MouseLocation.x - ViewportLocation.offset.x;
        float MouseY = MouseLocation.y - ViewportLocation.offset.y;

        HitscanParams params;
        WorldUtility::HitscanCamToWorld(Editor::Get()->GetWorld(), MouseX, MouseY, params);
    }

    return false;
}