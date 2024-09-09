#pragma once

#include <RenderV/Layer.h>

class Widget;

class UIRenderLayerRef : public IRenderLayerRef
{
protected:
    Widget* widget;
public:
    UIRenderLayerRef();

    UIRenderLayerRef* SetCanvasWidget(Widget* newWidget);

    void onCanvasWidgetBind(){}
};

class UIRenderLayer : public IRenderLayer
{
public:
    static IRenderLayerRef* CreateRef() {return new UIRenderLayerRef;}

    static std::string GetStaticName() {return "UIRenderLayer";}
};