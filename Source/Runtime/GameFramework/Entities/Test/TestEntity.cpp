#include "TestEntity.h"
#include <RenderV/Utility.h>
#include <Platform/Platform.h>
#include <World/RenderLayer/WorldRenderLayer.h>

EntityRenderProxyTest::EntityRenderProxyTest(Entity* DefaultParent)
    : EntityRenderProxy(DefaultParent)
{
}

void EntityRenderProxyTest::CreateResources(WorldRenderLayerRef* layerRef)
{
    if(shader.get() != nullptr && shader->IsValid())
    {
        return;
    }
    std::unique_ptr<IFile> VertFile = IPlatform::Get()->OpenFile("./Content/Shaders/Test/Test.vert.spv", FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY | FILE_ACCESS_FLAG_ATE);
    std::unique_ptr<IFile> FragFile = IPlatform::Get()->OpenFile("./Content/Shaders/Test/Test.frag.spv", FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY | FILE_ACCESS_FLAG_ATE);

    ShaderDescriptorLayout descriptorsLayout;
    descriptorsLayout.GenerateBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

    shader = std::make_shared<IShader>(layerRef->GetParentLayer()->GetRenderPass(), VertFile->FetchAllBinary(), FragFile->FetchAllBinary(), descriptorsLayout);

    material = std::make_shared<IMaterial>(shader.get());
}

void EntityRenderProxyTest::Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef)
{
    IRenderUtility::BeginDebugLabel(cmdBuffer, "Test");

    material->SetBuffer(0, layerRef->GetSceneDataBuffer()->GetBufferObject(), layerRef->GetSceneDataBuffer()->GetSize());
    VkDescriptorSet set = material->Get(layerRef->GetParentComposition()->GetCurrentImageIndex());
    vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipelineLayout(), 0, 1, &set, 0, nullptr);

    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipeline());

    vkCmdDraw(cmdBuffer, 3, 1, 0, 0);

    IRenderUtility::EndDebugLabel(cmdBuffer);
}