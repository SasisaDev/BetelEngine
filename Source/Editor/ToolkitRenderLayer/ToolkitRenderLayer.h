#pragma once

#include <glm/glm.hpp>

#include <RenderV/Composition.h>
#include <RenderV/Objects/Material.h>
#include <RenderV/Objects/Buffers/Buffer.h>

struct ToolsRenderLayerGPUStorage
{
    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;
};

class ToolkitRenderLayerRef : public IRenderLayerRef
{
    friend class UIRenderLayer;
protected:
    // GPU Storage Buffer Binding 0
    std::vector<ToolsRenderLayerGPUStorage> SceneDataStorages;
    std::vector<Buffer*> SceneDataSSBOs;
public:
    ToolkitRenderLayerRef();

    virtual bool Initialize(VkDevice device, RenderDependencyList<IRenderLayerRef>& DependencyList) override;

    inline Buffer* GetSceneDataBuffer() const {return SceneDataSSBOs[GetParentComposition()->GetCurrentImageIndex()];}

    void onCanvasWidgetBind(){}
};

class ToolkitRenderLayer : public IRenderLayer
{
public:
    static IRenderLayerRef* CreateRef() {return new ToolkitRenderLayerRef;}

    static std::string GetStaticName() {return "ToolkitRenderLayer";}

    virtual bool Initialize(VkDevice device) override;

    virtual void Prepare(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer) override;

    virtual void Render(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer) override;

    virtual bool Deinitialize() override;
};