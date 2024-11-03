#include "ImGuiLayer.h"
#include "../BetelImGui.h"

ImGuiRenderLayerRef::ImGuiRenderLayerRef()
{
    ImGuiE = new BetelImGuiEngine();
}

bool ImGuiRenderLayerRef::Initialize(VkDevice device, RenderDependencyList<IRenderLayerRef>& DependencyList)
{
    return true;
}

ImGuiRenderLayerRef* ImGuiRenderLayerRef::SetHostWindow(Window* window) {
    HostWindow = window;
    if(HostWindow == nullptr) {
        LOG(Fatal, LogImGui, "Host window for ImGui engine is not specified");
    }

    ImGui_ImplVulkan_InitInfo info{};
    //TODO: DescriptorPool Management
    //info.DescriptorPool = 
    ImGuiE->Initialize(HostWindow->GetSDLWindowHandle(), info);
    return this;
}

bool ImGuiRenderLayer::Initialize(VkDevice device)
{
    return true;
}

void ImGuiRenderLayer::Prepare(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer)
{

}

void ImGuiRenderLayer::Render(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer)
{
    ImGuiRenderLayerRef* uiRef = (ImGuiRenderLayerRef*)layerRef;
    uint32_t CurrentFrame = layerRef->GetParentComposition()->GetCurrentImageIndex(); 

    IRenderUtility::BeginDebugLabel(cmdBuffer, "ImGui User Interface", 0.85, 0.35, 0.35);

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmdBuffer);

    IRenderUtility::EndDebugLabel(cmdBuffer);


}

//TODO: void ImGuiRenderLayer::RenderWidget(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer, Widget* widget){}

bool ImGuiRenderLayer::Deinitialize()
{
    return true;
}