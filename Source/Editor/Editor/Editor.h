#pragma once

class World;
class Entity;
class Component;
class EditorGizmo;

class Editor {
protected:
    EditorGizmo* Gizmo;
protected:
    World* CurrentWorld;
    Entity* SelectedEntity;
public:
    Editor();

    static Editor* Get() {
        static Editor* editor = new Editor;
        return editor;
    }
};
