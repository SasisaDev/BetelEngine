#pragma once
#pragma warning(disable:4455)

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

enum WidgetDirections {
    WIDGET_DIRECTION_CENTER,
    WIDGET_DIRECTION_FIRST,
    WIDGET_DIRECTION_LAST,
    WIDGET_DIRECTION_LEFT,
    WIDGET_DIRECTION_RIGHT,
    WIDGET_DIRECTION_TOP,
    WIDGET_DIRECTION_BOTTOM,
    WIDGET_DIRECTION_ROW,
    WIDGET_DIRECTION_COLUMN,
    WIDGET_DIRECTION_FLEX,
    WIDGET_DIRECTION_ABSOLUTE,
    WIDGET_DIRECTION_RELATIVE
};

struct WidgetSlotTransform {
    float x, y;
    float width, height;
    float z = 0;
};

struct WidgetUnit {
    enum class Type : uint8_t {
        Pixel,
        Percent,
        FitContent
    } type;
    float value;

    static WidgetUnit FitContent;

    WidgetUnit(int value):type(WidgetUnit::Type::Pixel),value(value){}
    WidgetUnit():type(WidgetUnit::Type::Percent),value(100){}
    WidgetUnit(WidgetUnit::Type typ, float val):type(typ), value(val){}

    inline bool IsFitContent() const {return type == WidgetUnit::Type::FitContent;}

    float GetPixelValue(float BaseValue) {
        if(type == WidgetUnit::Type::Pixel) {
            return value;
        }
        return BaseValue*value;
    }
};

class Widget
{
    friend class UIRenderLayer;
protected:
    WidgetSlotAlignFlags align;
    WidgetSlotStretchFlags stretch;

    WidgetUnit MarginL = 0, MarginR = 0, MarginB = 0, MarginT = 0;
    WidgetUnit PaddingL = 0, PaddingR = 0, PaddingB = 0, PaddingT = 0;

    WidgetUnit Width, Height;

    uint8_t JustifyContent = WIDGET_DIRECTION_FIRST, AlignContent = WIDGET_DIRECTION_FIRST;
    uint8_t Direction = WIDGET_DIRECTION_ROW;
    uint8_t Position = WIDGET_DIRECTION_FLEX;

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

    virtual Widget* SetMargin(WidgetUnit&& Margin) {MarginL = MarginR = MarginT = MarginB = Margin; return this;}
    virtual Widget* SetMargin(WidgetUnit&& Left, WidgetUnit&& Right, WidgetUnit&& Top, WidgetUnit&& Bottom) {MarginL = Left; MarginR = Right; MarginT = Top; MarginB = Bottom; return this;}
    virtual Widget* SetMargin(WidgetUnit&& Horizontal, WidgetUnit&& Vertical) {MarginL = MarginR = Horizontal; MarginT = MarginB = Vertical; return this;}
    virtual Widget* SetMarginLeft(WidgetUnit&& Margin) {MarginL = Margin; return this;}
    virtual Widget* SetMarginRight(WidgetUnit&& Margin) {MarginR = Margin; return this;}
    virtual Widget* SetMarginTop(WidgetUnit&& Margin) {MarginT = Margin; return this;}
    virtual Widget* SetMarginBottom(WidgetUnit&& Margin) {MarginB = Margin; return this;}

    virtual Widget* SetPadding(WidgetUnit&& Padding) {PaddingL = PaddingR = PaddingT = PaddingB = Padding; return this;}
    virtual Widget* SetPadding(WidgetUnit&& Left, WidgetUnit&& Right, WidgetUnit&& Top, WidgetUnit&& Bottom) {PaddingL = Left; PaddingR = Right; PaddingT = Top; PaddingB = Bottom; return this;}
    virtual Widget* SetPadding(WidgetUnit&& Horizontal, WidgetUnit&& Vertical) {PaddingL = PaddingR = Horizontal; PaddingT = PaddingB = Vertical; return this;}
    virtual Widget* SetPaddingLeft(WidgetUnit&& Padding) {PaddingL = Padding; return this;}
    virtual Widget* SetPaddingRight(WidgetUnit&& Padding) {PaddingR = Padding; return this;}
    virtual Widget* SetPaddingTop(WidgetUnit&& Padding) {PaddingT = Padding; return this;}
    virtual Widget* SetPaddingBottom(WidgetUnit&& Padding) {PaddingB = Padding; return this;}

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