#include "ToolkitRenderLayer.h"
#include <Engine/EngineDelegates.h>
#include <Editor/EditorDelegates.h>

#include <functional>
#include <Log/Logger.h>
#include <RenderV/Composition.h>
#include <RenderV/Utility.h>
#include <Editor/Tools/Tool.h>

void ToolkitRenderLayerRef::AddToolCallback(EditorTool* tool) 
{
    if(tool) {
        Proxies.insert(tool);
        tool->CreateResources(this);
    }
}

void ToolkitRenderLayerRef::RemoveToolCallback(EditorTool* tool) 
{
    if(Proxies.contains(tool)) {
        Proxies.erase(Proxies.find(tool));
    }
}

ToolkitRenderLayerRef::ToolkitRenderLayerRef()
{

}

bool ToolkitRenderLayerRef::Initialize(VkDevice device, RenderDependencyList<IRenderLayerRef> &DependencyList)
{
    // Register for Editor Delegates
    EditorDelegates::ToolCreated.BindMember(this, &ToolkitRenderLayerRef::AddToolCallback);
    EditorDelegates::ToolBeginDestroy.BindMember(this, &ToolkitRenderLayerRef::RemoveToolCallback);

    return true;
}

bool ToolkitRenderLayer::Initialize(VkDevice device)
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
    // colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    // colorAttachmentRef.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

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

    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
    {
        LOG(Fatal, LogRender, "failed to create render pass!");
    }

    return true;
}

void ToolkitRenderLayer::Prepare(VkCommandBuffer cmdBuffer, IRenderLayerRef *layerRef, IRenderLayerRef *previousLayer)
{
    ToolkitRenderLayerRef *tkRef = (ToolkitRenderLayerRef *)layerRef;

}

void ToolkitRenderLayer::Render(VkCommandBuffer cmdBuffer, IRenderLayerRef *layerRef, IRenderLayerRef *previousLayer)
{
    if (layerRef->GetParentComposition()->GetGameViewport().extent.width == 0 ||
        layerRef->GetParentComposition()->GetGameViewport().extent.height == 0)
    {
        return;
    }

    ToolkitRenderLayerRef *tkRef = (ToolkitRenderLayerRef *)layerRef;
    uint32_t CurrentFrame = layerRef->GetParentComposition()->GetCurrentImageIndex();

    IRenderUtility::BeginDebugLabel(cmdBuffer, "Editor Toolkit", 0.35, 0.35, 0.85);

    VkRenderPassBeginInfo passInfo;
    passInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    passInfo.pNext = nullptr;
    passInfo.renderPass = renderPass;
    passInfo.clearValueCount = 0;
    passInfo.framebuffer = layerRef->GetParentComposition()->GetCurrentFramebuffer();
    passInfo.renderArea.offset = tkRef->GetParentComposition()->GetOffset();
    passInfo.renderArea.extent = tkRef->GetParentComposition()->GetExtent();

    vkCmdBeginRenderPass(cmdBuffer, &passInfo, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport;

    viewport.x = layerRef->GetParentComposition()->GetGameViewport().offset.x;
    viewport.y = layerRef->GetParentComposition()->GetGameViewport().offset.y;
    viewport.width = layerRef->GetParentComposition()->GetGameViewport().extent.width;
    viewport.height = layerRef->GetParentComposition()->GetGameViewport().extent.height;
    viewport.minDepth = 0;
    viewport.maxDepth = 1;

    VkRect2D scissors;

    scissors.offset = layerRef->GetParentComposition()->GetGameViewport().offset;
    scissors.extent = layerRef->GetParentComposition()->GetGameViewport().extent;

    vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);
    vkCmdSetScissor(cmdBuffer, 0, 1, &scissors);

    // TODO: RENDER HERE
    for(EditorTool* proxy : tkRef->Proxies) 
    {
        proxy->Render(cmdBuffer, tkRef);
    }

    vkCmdEndRenderPass(cmdBuffer);

    IRenderUtility::EndDebugLabel(cmdBuffer);
}

bool ToolkitRenderLayer::Deinitialize()
{
    return true;
}