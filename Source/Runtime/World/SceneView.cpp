#include "SceneView.h"

void SceneView::UpdateViewProjectionMatrix(glm::mat4 vpm)
{
    ViewProjectionMatrix = vpm;
    InvViewProjectionMatrix = glm::inverse(ViewProjectionMatrix);
}

void SceneView::UpdateViewProjectionMatrix()
{
    ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
    InvViewProjectionMatrix = glm::inverse(ViewProjectionMatrix);
}

void SceneView::UpdateViewMatrix(glm::mat4 vm)
{
    ViewMatrix = vm;
    InvViewMatrix = glm::inverse(ViewMatrix);
}

void SceneView::UpdateProjectionMatrix(glm::mat4 pm)
{
    ProjectionMatrix = pm;
    InvProjectionMatrix = glm::inverse(ProjectionMatrix);
}
