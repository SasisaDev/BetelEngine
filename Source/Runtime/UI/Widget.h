#pragma once

#include <vector>
#include <memory>
#include "WidgetBrush.h"
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
    float width, height;
    float z = 0;
};

struct WidgetUnit {
    enum class Type : uint8_t {
        Pixel,
        Percent
    } type;
    float value;

    WidgetUnit():type(WidgetUnit::Type::Pixel),value(0){}
    WidgetUnit(WidgetUnit::Type typ, float val):type(typ), value(val){}
};

extern WidgetUnit operator "" per(long double value);
extern WidgetUnit operator "" px(long double value);

extern WidgetUnit operator "" per(unsigned long long int value);
extern WidgetUnit operator "" px(unsigned long long int value);

class Widget
{
    friend class UIRenderLayer;
protected:
    WidgetSlotAlignFlags align;
    WidgetSlotStretchFlags stretch;

    uint16_t MarginL = 0, MarginR = 0, MarginB = 0, MarginT = 0;
    uint16_t PaddingL = 0, PaddingR = 0, PaddingB = 0, PaddingT = 0;

    WidgetUnit Width, Height;

    WidgetSlotTransform transform;

    bool externallyConstrained = false;

    std::vector<std::shared_ptr<Widget>> children;

    Widget* parent;
public:

    virtual void RecalculateChildrenTransforms();

    virtual void SetParent(Widget* widget) {parent = widget;}
    virtual Widget* GetParent() const {return parent;}
    
    virtual void Tick(float deltaTime){}

    virtual Widget* SetTransform(WidgetSlotTransform slotTransform) {transform = slotTransform; return this;}
    virtual Widget* SetAlign(WidgetSlotAlignFlags flags) {align = flags; return this;}
    virtual Widget* SetStretch(WidgetSlotStretchFlags flags) {stretch = flags; return this;}

    virtual Widget* SetMargin(uint16_t Margin) {MarginL = MarginR = MarginT = MarginB = Margin; return this;}
    virtual Widget* SetMargin(uint16_t Left, uint16_t Right, uint16_t Top, uint16_t Bottom) {MarginL = Left; MarginR = Right; MarginT = Top; MarginB = Bottom; return this;}
    virtual Widget* SetMargin(uint16_t Horizontal, uint16_t Vertical) {MarginL = MarginR = Horizontal; MarginT = MarginB = Vertical; return this;}
    virtual Widget* SetMarginLeft(uint16_t Margin) {MarginL = Margin; return this;}
    virtual Widget* SetMarginRight(uint16_t Margin) {MarginR = Margin; return this;}
    virtual Widget* SetMarginTop(uint16_t Margin) {MarginT = Margin; return this;}
    virtual Widget* SetMarginBottom(uint16_t Margin) {MarginB = Margin; return this;}

    virtual Widget* SetPadding(uint16_t Padding) {PaddingL = PaddingR = PaddingT = PaddingB = Padding; return this;}
    virtual Widget* SetPadding(uint16_t Left, uint16_t Right, uint16_t Top, uint16_t Bottom) {PaddingL = Left; PaddingR = Right; PaddingT = Top; PaddingB = Bottom; return this;}
    virtual Widget* SetPadding(uint16_t Horizontal, uint16_t Vertical) {PaddingL = PaddingR = Horizontal; PaddingT = PaddingB = Vertical; return this;}
    virtual Widget* SetPaddingLeft(uint16_t Padding) {PaddingL = Padding; return this;}
    virtual Widget* SetPaddingRight(uint16_t Padding) {PaddingR = Padding; return this;}
    virtual Widget* SetPaddingTop(uint16_t Padding) {PaddingT = Padding; return this;}
    virtual Widget* SetPaddingBottom(uint16_t Padding) {PaddingB = Padding; return this;}

    virtual Widget* SetWidth(WidgetUnit&& width) {Width = width; return this;}
    virtual Widget* SetHeight(WidgetUnit&& height) {Height = height; return this;}

    virtual Widget* AddChild(std::shared_ptr<Widget>&& child) {
        if(child->GetParent()) {
            child->GetParent()->RemoveChild(child.get());
        }
        child->SetParent(this);
        children.push_back(child);
        return this;
    }

    virtual void RemoveChild(Widget* widget) {
        for(auto i = 0; i < children.size(); ++i) {
            if(children[i].get() == widget) {
                children.erase(children.begin()+i);
                return;
            }
        }
    }

    virtual Vec2 GetExtent(){return {1280, 720};}
    virtual Vec2 GetOffset(){return {1280, 720};}
    virtual Vec4 GetBounds(){return {};}

    virtual void CreateResources(){}
    virtual void Render(VkCommandBuffer cmdBuffer){}
};