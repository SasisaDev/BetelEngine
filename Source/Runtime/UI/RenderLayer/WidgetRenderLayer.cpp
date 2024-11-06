#include "WidgetRenderLayer.h"
#include <Engine/EngineDelegates.h>
#include "../Widget.h"
#include <functional>
#include <Log/Logger.h>
#include <RenderV/Composition.h>
#include <RenderV/Utility.h>

UIRenderLayerRef::UIRenderLayerRef()
{
    
}

bool UIRenderLayerRef::Initialize(VkDevice device, RenderDependencyList<IRenderLayerRef>& DependencyList)
{
    
    return true;
}

UIRenderLayerRef* UIRenderLayerRef::SetCanvasWidget(Widget* newWidget)
{
    widget = newWidget;
    return this;
}

bool UIRenderLayer::Initialize(VkDevice device)
{
    // Create Render Pass
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = VK_FORMAT_B8G8R8A8_SRGB;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    //colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    //colorAttachmentRef.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        LOG(Fatal, LogRender, "failed to create render pass!");
    }

    return true;
}

void UIRenderLayer::Prepare(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer)
{

}

void UIRenderLayer::Render(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer)
{
    UIRenderLayerRef* uiRef = (UIRenderLayerRef*)layerRef;
    uint32_t CurrentFrame = layerRef->GetParentComposition()->GetCurrentImageIndex(); 

    IRenderUtility::BeginDebugLabel(cmdBuffer, "User Interface", 0.35, 0.35, 0.85);

    VkRenderPassBeginInfo passInfo;
    passInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    passInfo.pNext = nullptr;
    passInfo.renderPass = renderPass;
    passInfo.clearValueCount = 0;
    passInfo.framebuffer = layerRef->GetParentComposition()->GetCurrentFramebuffer();
    passInfo.renderArea.offset = layerRef->GetParentComposition()->GetGameViewport().offset;
    passInfo.renderArea.extent = layerRef->GetParentComposition()->GetGameViewport().extent;

    vkCmdBeginRenderPass(cmdBuffer, &passInfo, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

    // TODO: Make recursive children tree rendering
    if(Widget* canvas = uiRef->widget)
    {
        VkViewport viewport;

        viewport.x = layerRef->GetParentComposition()->GetGameViewport().offset.x;
        viewport.y = layerRef->GetParentComposition()->GetGameViewport().offset.y;
        viewport.width = layerRef->GetParentComposition()->GetGameViewport().extent.width;
        viewport.height = layerRef->GetParentComposition()->GetGameViewport().extent.height;
        viewport.minDepth = 0;
        viewport.maxDepth = 1;

        /* TODO: Widget Viewport Based Rendering
        viewport.x = 0;
        viewport.y = 0;
        viewport.width = canvas->GetGameViewport().extent.x;
        viewport.height = canvas->GetGameViewport().extent.y;
        //viewport.width = layerRef->GetParentComposition()->GetGameViewport().extent.width;
        //viewport.height = layerRef->GetParentComposition()->GetGameViewport().extent.height;
        viewport.minDepth = 0;
        viewport.maxDepth = 1;
        */

        VkRect2D scissors;
        // TODO: Widget Viewport Based Rendering
        /*
        VkExtent2D extent = {static_cast<uint32_t>(canvas->GetGameViewport().extent.x), static_cast<uint32_t>(canvas->GetGameViewport().extent.y)};

        scissors.offset = {0, 0};
        scissors.extent = extent;
        */

        scissors.offset = layerRef->GetParentComposition()->GetGameViewport().offset;
        scissors.extent = layerRef->GetParentComposition()->GetGameViewport().extent;

        vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);
        vkCmdSetScissor(cmdBuffer, 0, 1, &scissors);

        Widget* renderTargetWidget = uiRef->widget;
        RenderWidget(cmdBuffer, layerRef, previousLayer, renderTargetWidget);
    }

    vkCmdEndRenderPass(cmdBuffer);

    IRenderUtility::EndDebugLabel(cmdBuffer);


}

void UIRenderLayer::RenderWidget(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer, Widget* widget)
{
    widget->Render(cmdBuffer);
    for(auto& widgetChild : widget->children) {
        RenderWidget(cmdBuffer, layerRef, previousLayer, widgetChild.get());
    }
}

bool UIRenderLayer::Deinitialize()
{
    return true;
}