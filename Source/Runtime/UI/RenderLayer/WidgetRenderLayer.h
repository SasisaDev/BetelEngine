#pragma once

#include <glm/glm.hpp>

#include <RenderV/Composition.h>
#include <RenderV/Objects/Material.h>
#include <RenderV/Objects/Buffers/Buffer.h>

class Widget;

struct UIRenderLayerGPUStorage
{
    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
};

class UIRenderLayerRef : public IRenderLayerRef
{
    friend class UIRenderLayer;
protected:
    Widget* widget;

    // GPU Storage Buffer Binding 0
    std::vector<UIRenderLayerGPUStorage> SceneDataStorages;
    std::vector<Buffer*> SceneDataSSBOs;
public:
    UIRenderLayerRef();

    virtual bool Initialize(VkDevice device, RenderDependencyList<IRenderLayerRef>& DependencyList) override;

    inline Buffer* GetSceneDataBuffer() const {return SceneDataSSBOs[GetParentComposition()->GetCurrentImageIndex()];}

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