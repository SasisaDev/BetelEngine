#pragma once

#include "../../Widget.h"

class PanelWidget : public Widget
{
protected:
    //WidgetBrush brush;
public:

    virtual void Render(VkCommandBuffer cmdBuffer) override;
};