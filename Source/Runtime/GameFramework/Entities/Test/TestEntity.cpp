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
    IFile* VertFile = IPlatform::Get()->OpenFile("./Shaders/Test/Test.vert.spv", FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY | FILE_ACCESS_FLAG_ATE);
    IFile* FragFile = IPlatform::Get()->OpenFile("./Shaders/Test/Test.frag.spv", FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY | FILE_ACCESS_FLAG_ATE);

    ShaderDescriptorLayout descriptorsLayout;

    shader = std::make_shared<IShader>(layerRef->GetParentLayer()->GetRenderPass(), VertFile->FetchAllBinary(), FragFile->FetchAllBinary(), descriptorsLayout);
}

void EntityRenderProxyTest::Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef)
{
    IRenderUtility::BeginDebugLabel(cmdBuffer, "Test");

    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipeline());

    vkCmdDraw(cmdBuffer, 3, 1, 0, 0);

    IRenderUtility::EndDebugLabel(cmdBuffer);
}