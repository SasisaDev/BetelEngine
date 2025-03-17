#include "TestEntity.h"
#include <RenderV/Utility.h>
#include <Platform/Platform.h>
#include <World/RenderLayer/WorldRenderLayer.h>

#include <Engine/Engine.h>

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
    std::shared_ptr<Resource> VertFile = GEngine->GetAssetLoader()->LoadResource("Shaders/Test/Test.vert.spv");
    std::shared_ptr<Resource> FragFile = GEngine->GetAssetLoader()->LoadResource("Shaders/Test/Test.frag.spv");

    ShaderDescriptorLayout descriptorsLayout;
    descriptorsLayout.GenerateBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

    shader = std::make_shared<IShader>(layerRef->GetParentLayer()->GetRenderPass(), VertFile->GetBuffer(), FragFile->GetBuffer(), descriptorsLayout);

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

void EntityTest::Tick(float deltaTime)
{
    static float param = 0;
    param += deltaTime;
    double sin = glm::sin(param / 1.0) * 100.0;
    double cos = glm::cos(param / 1.5) * 100.0;
    SetRelativeLocation({static_cast<int>(sin) , static_cast<int>(cos) , transform.Location.z});
}
