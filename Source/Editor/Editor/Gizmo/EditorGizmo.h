#pragma once

#include <World/Entity.h>
#include <Math/Vector.h>

enum class EditorGizmoMode : uint8_t {
    Location,
    Rotation,
    Scale,
    Size
};

class EditorGizmo  {
    friend class Editor;
protected:
    Vec2 Position;
    EditorGizmoMode Mode;
    bool Visible = false;
public:
};

class EditorGizmoRenderProxy : public EntityRenderProxy
{
    EditorGizmo* Gizmo;
public:
    EditorGizmoRenderProxy(EditorGizmo* gizmo, void* sprite) : EntityRenderProxy(nullptr) {Gizmo = gizmo;}
    virtual void Update(WorldRenderLayerRef* layerRef) override {}
    virtual void Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef) override {

    }
};