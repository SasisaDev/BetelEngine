#pragma once

#include <vector>
#include <queue>
#include <memory>

#include <Math/Vector.h>
#include <Input/InputEvent.h>
#include <Toolkit/Toolkit.h>
#include <TransactionQueue/Context.h>
#include "Mode.h"

class World;
class Object;
class Entity;
class Component;
class EditorTool;

struct EditorInputContext
{
    // Dragging
    bool IsDragging = false;
    float MouseDragStartX = 0;
    float MouseDragStartY = 0;
    IVec2 OriginalEditorCameraPosition = {};
};

class Editor {
    Editor();

    // World travel
    void InternalTravel();
    World* travelWorld = nullptr;

    EditorToolkit *edToolkit;

    std::unique_ptr<TransactionContext> transactions;

    EditorInputContext edInputCtx;
    virtual void HandleIncomingInputEvent(InputEvent &event);
    virtual void EventLoadWorld(World* world);
protected:
    std::vector<EditorTool*> Tools;
protected:
    World* CurrentWorld = nullptr;
    Object* SelectedObject = nullptr;

    bool bViewportHovered = false;
    bool bInputConsumed = false;

    IVec2 EditorCameraPosition = {};
    IVec2 EditorCameraRotation = {};
public:
    float ViewportZoom = 1;
    bool bShowOverlay = true;

    std::queue<size_t> ModesRemoveQueue;
    std::vector<EditorMode*> Modes;
    std::vector<size_t> SelectedObjectModes;
    int CurrentActiveMode = 0, PreviousCurrentActiveMode = 0;

    /*
     * Returns static Editor instance pointer
     * First call will trigger the static initialization
     * And a constructor, which relies on 
     * GApplication being correct initialized, otherwise it will crash
     */
    static Editor* Get() {
        static Editor* editor = new Editor;
        return editor;
    }

    EditorToolkit* GetToolkitUI() {return edToolkit;}

    void TravelToWorld(World* world){travelWorld = world;}
    World* GetWorld() {return CurrentWorld;}

    Object* GetSelectedObject() const {return SelectedObject;}
    void SetSelectedObject(Object* selectedObject);

    void SetViewportHovered(bool isHovered) {bViewportHovered = isHovered;}
    bool GetViewportHovered() {return bViewportHovered;}

    void SetInputConsumed(bool isConsumed) {bInputConsumed = isConsumed;}
    bool GetInputConsumed() {return bInputConsumed;}

    // Can be used for Defered Registration
    template <typename ToolkitModeT>
    bool AddToolkitMode() {
        Modes.push_back(new ToolkitModeT);
        return true;
    }

    EditorMode* GetActiveToolkitMode() const {return Modes[CurrentActiveMode];}

    template <typename ToolkitModeT>
    bool QueueRemoveToolkitMode() {
        for(int i = 0; i < Modes.size(); i++)
        {
            if(dynamic_cast<ToolkitModeT*>(Modes[i]))
            {
                ModesRemoveQueue.push(i);
                return true;        
            }
        }
        return false;
    }

    virtual void Tick(float deltaTime);
};
