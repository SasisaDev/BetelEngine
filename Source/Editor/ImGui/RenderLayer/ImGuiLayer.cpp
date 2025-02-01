#include "ImGuiLayer.h"
#include "../BetelImGui.h"
#include <EditorUI/WindowLibrary/BetelImages.h>
#include <EditorUI/WindowLibrary/BetelDeferred.h>


ImGuiRenderLayerRef::ImGuiRenderLayerRef()
{
    ImGuiE = new BetelImGuiEngine();
}

bool ImGuiRenderLayerRef::Initialize(VkDevice device, RenderDependencyList<IRenderLayerRef>& DependencyList)
{
    // Amount of statically defined images used by Editor
    const size_t BetelEditorImageCount = static_cast<size_t>(BImGui::Img::MAX);

    VkDescriptorPoolCreateInfo descPoolInfo{};
    descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descPoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    descPoolInfo.maxSets = IRenderUtility::GetFramesInFlight() + MaxSetsCount + BetelEditorImageCount;

    VkDescriptorPoolSize descPoolSize;
    descPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descPoolSize.descriptorCount = IRenderUtility::GetFramesInFlight();
    descPoolInfo.poolSizeCount = 1;
    descPoolInfo.pPoolSizes = &descPoolSize;

    if(vkCreateDescriptorPool(IRenderUtility::GetDevice(), &descPoolInfo, 0, &imagesPool) != VK_SUCCESS) {
        LOG(Fatal, LogImGui, "Failed to create descriptor pool for ImGui.");
    }

    return true;
}

bool ImGuiRenderLayerRef::Deinitialize(VkDevice device)
{
    //EditorImageLoader::Get().FreeAllResources();
    delete ImGuiE;
    return true;
}

ImGuiRenderLayerRef* ImGuiRenderLayerRef::SetHostWindow(Window* window) {
    HostWindow = window;
    if(HostWindow == nullptr) {
        LOG(Fatal, LogImGui, "Host window for ImGui engine is not specified");
    }

    VkQueue queue;

    vkGetDeviceQueue(IRenderUtility::GetDevice(), IRenderUtility::FindQueueFamilies(IRenderUtility::GetPhysicalDevice()).graphicsFamily.value(), 0, &queue);

    ImGui_ImplVulkan_InitInfo info{};
    info.DescriptorPool = imagesPool;
    info.Device = IRenderUtility::GetDevice();
    info.PhysicalDevice = IRenderUtility::GetPhysicalDevice();
    info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    info.MinImageCount = info.ImageCount = IRenderUtility::GetFramesInFlight();
    info.RenderPass = GetParentLayer()->GetRenderPass();
    info.Instance = IRenderUtility::GetInstance();
    info.Queue = queue;
    
    ImGuiE->Initialize(HostWindow->GetSDLWindowHandle(), info);

    return this;
}

bool ImGuiRenderLayer::Initialize(VkDevice device)
{
    // Create Stretch Render Pass
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = VK_FORMAT_B8G8R8A8_SRGB;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    //colorAttachment.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
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
        LOG(Fatal, LogImGui, "failed to create render pass!");
    }

    return true;
}

void ImGuiRenderLayer::Prepare(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer)
{
    BImGui::FlushDeferredTasks();
}

void ImGuiRenderLayer::Render(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer)
{
    ImGuiRenderLayerRef* uiRef = (ImGuiRenderLayerRef*)layerRef;
    uint32_t CurrentFrame = layerRef->GetParentComposition()->GetCurrentImageIndex(); 

    IRenderUtility::BeginDebugLabel(cmdBuffer, "ImGui User Interface", 0.85, 0.35, 0.35);

    VkRenderPassBeginInfo passInfo;
    passInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    passInfo.pNext = nullptr;
    passInfo.renderPass = renderPass;
    passInfo.clearValueCount = 0;
    passInfo.framebuffer = layerRef->GetParentComposition()->GetCurrentFramebuffer();
    passInfo.renderArea.offset = {0, 0};
    passInfo.renderArea.extent = layerRef->GetParentComposition()->GetExtent();

    vkCmdBeginRenderPass(cmdBuffer, &passInfo, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    //ImGui::ShowDemoWindow();
    if(((ImGuiRenderLayerRef*)layerRef)->CurrentToolkit) {
        ((ImGuiRenderLayerRef*)layerRef)->CurrentToolkit->OnGUI(((ImGuiRenderLayerRef*)layerRef)->HostWindow);
    }

    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmdBuffer);

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    vkCmdEndRenderPass(cmdBuffer);

    IRenderUtility::EndDebugLabel(cmdBuffer);


}

bool ImGuiRenderLayer::Deinitialize()
{
    BImGui::RemoveAllDeferredTasks();
    return true;
}