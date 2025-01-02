#include "Editor.h"

#include <Core/Application/Application.h>

//#include "Gizmo/EditorGizmo.h"

Editor::Editor() {
    //Gizmo = new EditorGizmo();
}

void Editor::Tick(float deltaTime)
{
    if(GApplication)
    {
        if(World* world = GApplication->GetEngine()->GetWorld())
        {
            world->SetWorldCameraPosition(EditorCameraPosition);
        }
    }
}