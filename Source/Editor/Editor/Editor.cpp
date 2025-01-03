#include "Editor.h"

#include <Core/Application/Application.h>
#include <World/RenderLayer/WorldRenderLayer.h>

//#include "Gizmo/EditorGizmo.h"

void Editor::HandleIncomingInputEvent(InputEvent &event)
{
    // Enable / Disable dragging
    if(GApplication->GetEngine()->GetGameFocused()) {
        if(event.IsKey && event.IsMouse && event.MouseButton == SDL_BUTTON_MIDDLE)
        {
            edInputCtx.IsDragging = !event.IsUp;
            edInputCtx.MouseDragStartX = event.MouseX;
            edInputCtx.MouseDragStartY = event.MouseY;
            edInputCtx.OriginalEditorCameraPosition = EditorCameraPosition;
        }
    }

    if(edInputCtx.IsDragging) {
        if(event.IsMouse) {
            EditorCameraPosition.x = edInputCtx.OriginalEditorCameraPosition.x + (event.MouseX - edInputCtx.MouseDragStartX) / ViewportZoom;
            EditorCameraPosition.y = edInputCtx.OriginalEditorCameraPosition.y + (event.MouseY - edInputCtx.MouseDragStartY) / ViewportZoom;
        }    
    }

    // Zoom In/Out
    if(event.IsMouse && event.MouseWheel != 0 && GApplication->GetEngine()->GetGameFocused() && !edInputCtx.IsDragging)
    {
        ViewportZoom = std::clamp(ViewportZoom + (event.MouseWheel * (ViewportZoom / 5)), 0.05f, 10.f);
    }
}

Editor::Editor() {
    //Gizmo = new EditorGizmo();
    GApplication->GetEngine()->GetInputManager().OnInputEvent.BindMember(this, &Editor::HandleIncomingInputEvent);
}

void Editor::Tick(float deltaTime)
{
    if(World* world = GApplication->GetEngine()->GetWorld())
    {
        world->SetWorldCameraPosition(EditorCameraPosition);
    }
}