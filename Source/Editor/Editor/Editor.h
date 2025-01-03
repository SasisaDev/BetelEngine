#pragma once

#include <vector>
#include <Math/Vector.h>
#include <Input/InputEvent.h>

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
    Editor();

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

    virtual void Tick(float deltaTime);
};
