#include "WorldRenderLayer.h"
#include <Engine/EngineDelegates.h>
#include <functional>
#include <Log/Logger.h>
#include <RenderV/Utility.h>

#include <RenderV/Composition.h>

WorldRenderLayerRef::WorldRenderLayerRef()
{
    std::function<void(World*)> func;
    func = std::bind(&WorldRenderLayerRef::onWorldLoad, this, std::placeholders::_1);
    EngineDelegates::OnWorldLoad.AddBind(func);
}

WorldRenderLayerRef* WorldRenderLayerRef::SetWorld(World* newWorld)
{
    world = newWorld;
    return this;
}

bool WorldRenderLayer::Initialize(VkDevice device)
{
    // Create Render Pass
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = VK_FORMAT_B8G8R8A8_SRGB;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

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

void WorldRenderLayer::Prepare(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer)
{

}

void WorldRenderLayer::Render(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer)
{
    VkRenderPassBeginInfo passInfo;
    passInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    passInfo.pNext = nullptr;
    passInfo.renderPass = renderPass;
    VkClearValue clearVal = {{{0, 1, 0, 1}}};
    passInfo.pClearValues = &clearVal;
    passInfo.clearValueCount = 1;
    passInfo.framebuffer = layerRef->GetParentComposition()->GetCurrentFramebuffer();
    passInfo.renderArea.offset = {0, 0};
    passInfo.renderArea.extent =  layerRef->GetParentComposition()->GetExtent();

    vkCmdBeginRenderPass(cmdBuffer, &passInfo, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

    // Render
    VkViewport viewport;
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = layerRef->GetParentComposition()->GetExtent().width;
    viewport.height = layerRef->GetParentComposition()->GetExtent().height;

    vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

    // Render End

    vkCmdEndRenderPass(cmdBuffer);
}

bool WorldRenderLayer::Deinitialize()
{
    vkDestroyRenderPass(IRenderUtility::GetDevice(), renderPass, nullptr);

    return true;
}