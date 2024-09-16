#pragma once

#include "../../Widget.h"

class CanvasWidget : public Widget
{
protected:

public:

    virtual void Render(VkCommandBuffer cmdBuffer) override{}
};