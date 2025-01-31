#pragma once

#include <glm/glm.hpp>

#include <RenderV/Composition.h>
#include <RenderV/Objects/Material.h>
#include <RenderV/Objects/Buffers/Buffer.h>

#include <Core/Window/Window.h>

#include <Toolkit/Toolkit.h>

class BetelImGuiEngine;

class ImGuiRenderLayerRef : public IRenderLayerRef
{
    friend class ImGuiRenderLayer;
protected:
    uint32_t MaxSetsCount = 0;
    VkDescriptorPool imagesPool;

    BetelImGuiEngine* ImGuiE;
public:
    Window* HostWindow;
    EditorToolkit* CurrentToolkit;

    ImGuiRenderLayerRef();

    virtual bool Initialize(VkDevice device, RenderDependencyList<IRenderLayerRef>& DependencyList) override;
    virtual bool Deinitialize(VkDevice device) override;

    ImGuiRenderLayerRef* SetHostWindow(Window* window);
    ImGuiRenderLayerRef* SetToolkit(EditorToolkit* toolkit) {CurrentToolkit = toolkit; return this;}
    ImGuiRenderLayerRef* SetMaxSets(uint32_t maxSets) {MaxSetsCount = maxSets; return this;}

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

    virtual bool Deinitialize() override;
};