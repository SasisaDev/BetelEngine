#pragma once

#include <vector>
#include <Math/Vector.h>

class World;
class Entity;
class Component;
class EditorTool;

class Editor {
protected:
    std::vector<EditorTool*> Tools;
protected:
    World* CurrentWorld;
    Entity* SelectedEntity;

    IVec2 EditorCameraPosition = {};
    IVec2 EditorCameraRotation = {};
public:
    Editor();

    static Editor* Get() {
        static Editor* editor = new Editor;
        return editor;
    }

    virtual void Tick(float deltaTime) {}
};
