#pragma once

#include "../Tool.h"
#include <World/Entity.h>
#include <Math/Vector.h>

enum class EditorGizmoMode : uint8_t {
    Location,
    Rotation,
    Scale,
    Size
};

class EditorToolGizmo : public EditorTool
{
    friend class Editor;
protected:
    Vec2 Position;
    Vec2 SizeModeSize;
    EditorGizmoMode Mode;
    bool Visible = false;
public:
    virtual void Update(float deltaTime) override {}
    virtual void CreateResources(ToolkitRenderLayerRef* layerRef) override {}
    virtual void Render(VkCommandBuffer cmdBuffer, ToolkitRenderLayerRef* layerRef) override {}
};