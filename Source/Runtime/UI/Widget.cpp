#include "Widget.h"

WidgetUnit operator "" per(long double value) {
    return WidgetUnit(WidgetUnit::Type::Percent, static_cast<float>(value));
}

WidgetUnit operator "" px(long double value) {
    return WidgetUnit(WidgetUnit::Type::Pixel, static_cast<float>(value));
}

WidgetUnit operator "" per(unsigned long long int value) {
    return WidgetUnit(WidgetUnit::Type::Pixel, static_cast<float>(value));
}
WidgetUnit operator "" px(unsigned long long int value) {
    return WidgetUnit(WidgetUnit::Type::Pixel, static_cast<float>(value));
}

void Widget::RecalculateChildrenTransforms()
{
    
}