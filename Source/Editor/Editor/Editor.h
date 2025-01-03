#pragma once

#include <vector>
#include <Math/Vector.h>
#include <Input/InputEvent.h>
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

    EditorInputContext edInputCtx;
    virtual void HandleIncomingInputEvent(InputEvent &event);
protected:
    std::vector<EditorTool*> Tools;
protected:
    World* CurrentWorld;
    Entity* SelectedEntity;

    IVec2 EditorCameraPosition = {};
    IVec2 EditorCameraRotation = {};
public:
    float ViewportZoom = 1;
    bool bShowOverlay = true;

    std::vector<EditorMode*> Modes;
    int CurrentActiveMode = 0;

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

    // Can be used for Defered Registration
    template <typename ToolkitModeT>
    bool AddToolkitMode() {
        Modes.push_back(new ToolkitModeT);
        return true;
    }

    virtual void Tick(float deltaTime);
};
