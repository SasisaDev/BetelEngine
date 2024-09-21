#include "Widget.h"

WidgetUnit operator "" per(long double value) {
    return WidgetUnit(WidgetUnit::Type::Percent, static_cast<float>(value/100.0));
}

WidgetUnit operator "" px(long double value) {
    return WidgetUnit(WidgetUnit::Type::Pixel, static_cast<float>(value));
}

WidgetUnit operator "" per(unsigned long long int value) {
    return WidgetUnit(WidgetUnit::Type::Pixel, static_cast<float>(value/100.0));
}
WidgetUnit operator "" px(unsigned long long int value) {
    return WidgetUnit(WidgetUnit::Type::Pixel, static_cast<float>(value));
}

void Widget::RecalculateChildrenTransforms()
{
    bool bAlignFirst = (AlignContent==WIDGET_DIRECTION_FIRST), bAlignCenter = (AlignContent==WIDGET_DIRECTION_CENTER);
    bool bJustifyFirst = (JustifyContent==WIDGET_DIRECTION_FIRST), bJustifyCenter = (JustifyContent==WIDGET_DIRECTION_CENTER);
    bool bColumns = (Direction==WIDGET_DIRECTION_COLUMN);

    for(auto& child : children) {
        child->transform.x = transform.x + child->MarginL.GetPixelValue(transform.width) + PaddingL.GetPixelValue(transform.width);
        child->transform.y = transform.y + child->MarginT.GetPixelValue(transform.height) + PaddingT.GetPixelValue(transform.height);

        child->RecalculateChildrenTransforms();
    }
}