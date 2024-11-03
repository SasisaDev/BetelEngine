#pragma once

#include <glm/glm.hpp>

#include <RenderV/Composition.h>
#include <RenderV/Objects/Material.h>
#include <RenderV/Objects/Buffers/Buffer.h>

#include <Core/Window/Window.h>

class BetelImGuiEngine;

class ImGuiRenderLayerRef : public IRenderLayerRef
{
    friend class ImGuiRenderLayer;
protected:
    VkDescriptorPool imagesPool;

    BetelImGuiEngine* ImGuiE;
public:
    Window* HostWindow;

    ImGuiRenderLayerRef();

    virtual bool Initialize(VkDevice device, RenderDependencyList<IRenderLayerRef>& DependencyList) override;

    ImGuiRenderLayerRef* SetHostWindow(Window* window);

    void onCanvasWidgetBind(){}
};

class ImGuiRenderLayer : public IRenderLayer
{
public:
    static IRenderLayerRef* CreateRef() {return new ImGuiRenderLayerRef;}

    static std::string GetStaticName() {return "ImGuiRenderLayer";}

    virtual bool Initialize(VkDevice device) override;

    virtual void Prepare(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer) override;

    virtual void Render(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer) override;
    //TODO: virtual void RenderWidget(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer);

    virtual bool Deinitialize() override;
};