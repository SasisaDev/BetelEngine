#include "Editor.h"

#include <Core/Application/Application.h>
#include <World/RenderLayer/WorldRenderLayer.h>
#include <Engine/EngineDelegates.h>
#include <EditorUI/EditorToolkit.h>

#include "Modes/ObjectMode.h"

#include <glm/ext.hpp>

#include <algorithm>

//#include "Gizmo/EditorGizmo.h"

void Editor::HandleIncomingInputEvent(InputEvent &event)
{
    // Perform active dragging if it's already enabled
    if(edInputCtx.IsDragging && event.IsMouse && !event.IsKey && !event.MouseWheel) {
        EditorCameraPosition.x = edInputCtx.OriginalEditorCameraPosition.x - (event.MouseX - edInputCtx.MouseDragStartX) / ViewportZoom;
        EditorCameraPosition.y = edInputCtx.OriginalEditorCameraPosition.y + (event.MouseY - edInputCtx.MouseDragStartY) / ViewportZoom;
    }    

    // Next blocks require cursor hovering over focused viewport
    // Do nothing if Game Viewport is not focused
    if(!GApplication->GetEngine()->GetGameFocused() || !GetViewportHovered() || !GApplication->GetEngine()->GetWorld()) {
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

    // Zoom In/Out
    if(event.IsMouse && event.MouseWheel != 0)
    {
        ViewportZoom = std::clamp(ViewportZoom + (event.MouseWheel * (ViewportZoom / 5)), 0.05f, 10.f);
        if(CurrentWorld) {
            CurrentWorld->GetSceneView().UpdateViewMatrix(glm::scale(glm::mat4(1), glm::vec3(ViewportZoom, ViewportZoom, 1)));
        }
    }
}

void Editor::EventLoadWorld(World* world)
{
    CurrentWorld = world;
}

Editor::Editor() {
    edToolkit = new EditorToolkitBase;

    //Gizmo = new EditorGizmo();
    GApplication->GetEngine()->GetInputManager()->OnInputEvent.BindMember(this, &Editor::HandleIncomingInputEvent);
    EngineDelegates::OnWorldLoad.BindMember(this, &Editor::EventLoadWorld);

    CurrentWorld = GApplication->GetEngine()->GetWorld();

    // Add default mode
    AddToolkitMode<ObjectMode>();

    // Create transcation context
    // TODO: transcations memory settings
    transactions = std::make_unique<TransactionContext>(16*1024);
    transactions->Record<Transaction>(Transaction());
}

void Editor::InternalTravel()
{
    EditorCameraPosition = {0, 0};
    EditorCameraRotation = {0, 0};
    ViewportZoom = 1.f;

    if(GEngine->GetWorld()){
        EngineDelegates::OnWorldUnload.Broadcast(GEngine->GetWorld());
        //objectLibrary->UnloadObject(world->GetID());
    }
    
    // TODO: Create world's deep copy

    GEngine->SetWorld(travelWorld);
    GEngine->GetWorld()->PostInit();

    EngineDelegates::OnWorldLoad.Broadcast(GEngine->GetWorld());

    LOGF(Log, LogEngine, "Loaded world: 0x%08X.", GEngine->GetWorld()->GetID());

    travelWorld = nullptr;
}

void Editor::SetSelectedObject(Object* selectedObject)
{
    for(size_t mode : SelectedObjectModes) {
        ModesRemoveQueue.push(mode);
    }

    SelectedObjectModes.clear();

    SelectedObject = selectedObject;
    
    if(SelectedObject != nullptr) {
        if(Entity* entity = dynamic_cast<Entity*>(SelectedObject)){
            for(EditorMode* mode : entity->GetEditorModes()) {
                SelectedObjectModes.push_back(Modes.size());
                Modes.push_back(mode);
            }
        }
    }
}

void Editor::Tick(float deltaTime)
{
    // Load world on travel even in editor
    if(travelWorld) {
       InternalTravel();
    }

    // Override world cameras with Editor camera
    if(World* world = GApplication->GetEngine()->GetWorld())
    {
        world->GetSceneView().ViewOrigin = EditorCameraPosition;
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