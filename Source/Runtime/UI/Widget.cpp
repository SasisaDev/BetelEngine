#include "Widget.h"

WidgetUnit WidgetUnit::FitContent = WidgetUnit(WidgetUnit::Type::FitContent, 0);

void Widget::RecalculateChildrenTransforms()
{
    bool bAlignFirst = (AlignContent==WIDGET_DIRECTION_FIRST), bAlignCenter = (AlignContent==WIDGET_DIRECTION_CENTER);
    bool bJustifyFirst = (JustifyContent==WIDGET_DIRECTION_FIRST), bJustifyCenter = (JustifyContent==WIDGET_DIRECTION_CENTER);
    bool bColumns = (Direction==WIDGET_DIRECTION_COLUMN);

    for(size_t childIdx = 0; childIdx < children.size(); ++childIdx) {
        auto& child = children[childIdx];

        child->transform.x = transform.x + child->MarginL.GetPixelValue(transform.width) + PaddingL.GetPixelValue(transform.width);
        child->transform.y = transform.y + child->MarginT.GetPixelValue(transform.height) + PaddingT.GetPixelValue(transform.height);

        child->RecalculateChildrenTransforms();
    }
}