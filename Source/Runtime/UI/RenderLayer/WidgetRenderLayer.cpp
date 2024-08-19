#include "WidgetRenderLayer.h"
#include <Engine/EngineDelegates.h>
#include "../Widget.h"
#include <functional>

UIRenderLayerRef::UIRenderLayerRef()
{
    
}

UIRenderLayerRef* UIRenderLayerRef::SetCanvasWidget(Widget* newWidget)
{
    widget = newWidget;
    return this;
}