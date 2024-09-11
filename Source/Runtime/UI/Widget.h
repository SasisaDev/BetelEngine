#pragma once

#include <vector>
#include <memory>
#include <Math/Vector.h>

class Widget;

enum WidgetSlotAlignFlags {
    WIDGET_SLOT_ALIGN_CENTER = (1 << 0),
    WIDGET_SLOT_ALIGN_LEFT = (1 << 1),
    WIDGET_SLOT_ALIGN_RIGHT = (1 << 2),
    WIDGET_SLOT_ALIGN_TOP = (1 << 3),
    WIDGET_SLOT_ALIGN_BOTTOM = (1 << 4),
};

enum WidgetSlotStretchFlags {
    WIDGET_SLOT_STRETCH_HORIZONTAL = (1 << 0),
    WIDGET_SLOT_STRETCH_VERTICAL = (1 << 1),
};

struct WidgetSlotTransform {
    float x, y;
    float offsetX, offsetY;
    float z;
};

class Widget
{
    friend class UIRenderLayer;
protected:
    WidgetSlotAlignFlags align;
    WidgetSlotStretchFlags stretch;

    WidgetSlotTransform transform;

    bool externallyConstrained = false;

    std::vector<std::shared_ptr<Widget>> children;

    Widget* parent;
public:

    virtual void UpdateTransform();

    virtual void SetParent(Widget* widget) {parent = widget;}
    
    virtual void Tick(float deltaTime);

    virtual Widget* SetTransform(WidgetSlotTransform slotTransform) {transform = slotTransform; return this;}
    virtual Widget* SetAlign(WidgetSlotAlignFlags flags) {align = flags; return this;}
    virtual Widget* SetStretch(WidgetSlotStretchFlags flags) {stretch = flags; return this;}

    virtual Widget* AddChild(std::shared_ptr<Widget> child) {
        child->SetParent(this);
        children.push_back(child);
        return this;
    }

    virtual void Render();
};