#include "Editor.h"

#include <Core/Application/Application.h>
#include <World/RenderLayer/WorldRenderLayer.h>
#include <Engine/EngineDelegates.h>

#include "Modes/ObjectMode.h"

#include <algorithm>

//#include "Gizmo/EditorGizmo.h"

void Editor::HandleIncomingInputEvent(InputEvent &event)
{
    // Do nothing if Game Viewport is not focused
    if(!GApplication->GetEngine()->GetGameFocused()) {
        return;
    }

    // Pass Input to Editor Mode
    // If returned true, input is consumed
    if(Modes[CurrentActiveMode]->HandleInputEvent(event)) {
        return;
    }

    // Enable / Disable dragging
    if(event.IsKey && event.IsMouse && event.MouseButton == SDL_BUTTON_MIDDLE)
    {
        edInputCtx.IsDragging = !event.IsUp;
        edInputCtx.MouseDragStartX = event.MouseX;
        edInputCtx.MouseDragStartY = event.MouseY;
        edInputCtx.OriginalEditorCameraPosition = EditorCameraPosition;
    }

    if(edInputCtx.IsDragging && event.IsMouse && !event.IsKey && !event.MouseWheel) {
        EditorCameraPosition.x = edInputCtx.OriginalEditorCameraPosition.x + (event.MouseX - edInputCtx.MouseDragStartX) / ViewportZoom;
        EditorCameraPosition.y = edInputCtx.OriginalEditorCameraPosition.y + (event.MouseY - edInputCtx.MouseDragStartY) / ViewportZoom;
    }    

    // Zoom In/Out
    if(event.IsMouse && event.MouseWheel != 0)
    {
        ViewportZoom = std::clamp(ViewportZoom + (event.MouseWheel * (ViewportZoom / 5)), 0.05f, 10.f);
    }
}

void Editor::EventLoadWorld(World* world)
{
    CurrentWorld = world;
}

Editor::Editor() {
    //Gizmo = new EditorGizmo();
    GApplication->GetEngine()->GetInputManager().OnInputEvent.BindMember(this, &Editor::HandleIncomingInputEvent);
    EngineDelegates::OnWorldLoad.BindMember(this, &Editor::EventLoadWorld);

    CurrentWorld = GApplication->GetEngine()->GetWorld();

    // Add default mode
    AddToolkitMode<ObjectMode>();
}

void Editor::SetSelectedEntity(Entity* selectedEntity)
 {
    if(SelectedEntity != nullptr) {
        SelectedEntity->PopEditorModes(this);
    }

    SelectedEntity = selectedEntity;
    
    if(SelectedEntity != nullptr) {
        SelectedEntity->PushEditorModes(this);
    }
 }

void Editor::Tick(float deltaTime)
{
    if(World* world = GApplication->GetEngine()->GetWorld())
    {
        world->SetWorldCameraPosition(EditorCameraPosition);
    }

    // Remove all Modes in a Remove Queue
    if(ModesRemoveQueue.size() > 0) {
        CurrentActiveMode = 0;
        PreviousCurrentActiveMode = 0;
        while(ModesRemoveQueue.size()) {
            const size_t idToRemove = ModesRemoveQueue.back();
            // Switch to a safe default mode, that's existance is guaranteed 
            if(idToRemove == CurrentActiveMode)
            {
                CurrentActiveMode = 0;
                PreviousCurrentActiveMode = 0;
                Modes[idToRemove]->OnDisabled();
                Modes[CurrentActiveMode]->OnEnabled();
            }
            else if(idToRemove < CurrentActiveMode)
            {
                // Compensate for element removal
                PreviousCurrentActiveMode = CurrentActiveMode = CurrentActiveMode - 1;
            }

            // Remove Mode
            Modes.erase(Modes.begin() + idToRemove);
            
            ModesRemoveQueue.pop();
        }

    }

    if(CurrentActiveMode != PreviousCurrentActiveMode)
    {
        Modes[CurrentActiveMode]->OnEnabled();
        Modes[PreviousCurrentActiveMode]->OnDisabled();
        PreviousCurrentActiveMode = CurrentActiveMode;
    }

    Modes[CurrentActiveMode]->Tick(deltaTime);
}