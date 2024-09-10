#pragma once

#include <RenderV/Layer.h>

class Widget;

class UIRenderLayerRef : public IRenderLayerRef
{
    friend class UIRenderLayer;
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

    virtual bool Initialize(VkDevice device) override;

    virtual void Prepare(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer) override;

    virtual void Render(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer) override;

    virtual bool Deinitialize() override;
};