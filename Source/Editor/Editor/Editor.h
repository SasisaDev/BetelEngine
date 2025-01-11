#pragma once

#include <vector>
#include <queue>
#include <Math/Vector.h>
#include <Input/InputEvent.h>
#include <Toolkit/Toolkit.h>
#include "Mode.h"

class World;
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

    EditorToolkit *edToolkit;

    EditorInputContext edInputCtx;
    virtual void HandleIncomingInputEvent(InputEvent &event);
    virtual void EventLoadWorld(World* world);
protected:
    std::vector<EditorTool*> Tools;
protected:
    World* CurrentWorld = nullptr;
    Entity* SelectedEntity = nullptr;

    IVec2 EditorCameraPosition = {};
    IVec2 EditorCameraRotation = {};
public:
    float ViewportZoom = 1;
    bool bShowOverlay = true;

    std::queue<size_t> ModesRemoveQueue;
    std::vector<EditorMode*> Modes;
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

    World* GetWorld() {return CurrentWorld;}

    Entity* GetSelectedEntity() const {return SelectedEntity;}
    void SetSelectedEntity(Entity* selectedEntity);

    // Can be used for Defered Registration
    template <typename ToolkitModeT>
    bool AddToolkitMode() {
        Modes.push_back(new ToolkitModeT);
        return true;
    }

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
