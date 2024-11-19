#pragma once

#include <RenderV/Vulkan/vkloader.h>
class ToolkitRenderLayerRef;

class EditorTool {
public:
    bool bVisible = true;
public:
    virtual void Update(float deltaTime) {}

    virtual void CreateResources(ToolkitRenderLayerRef* layerRef){}
    virtual void Render(VkCommandBuffer cmdBuffer, ToolkitRenderLayerRef* layerRef) {}
};