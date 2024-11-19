#pragma once

#include <vector>

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
public:
    Editor();

    static Editor* Get() {
        static Editor* editor = new Editor;
        return editor;
    }
};
