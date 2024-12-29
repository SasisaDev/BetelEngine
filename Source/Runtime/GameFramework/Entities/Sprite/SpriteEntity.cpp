#include "SpriteEntity.h"
#include <World/RenderLayer/WorldRenderLayer.h>
#include <stb/stb_image.h>

void SpriteRenderProxy::CreateResources(WorldRenderLayerRef* layerRef)
{
    if(shader.get() != nullptr && shader->IsValid())
    {
        return;
    }
    Resource *VertShader = AssetLoader::Get().LoadResource("Shaders/SpriteMasked/SpriteMasked.vert.spv");
    Resource *FragShader = AssetLoader::Get().LoadResource("Shaders/SpriteMasked/SpriteMasked.frag.spv");

    if(VertShader == nullptr || FragShader == nullptr)
    {
        LOG(Fatal, LogWorldRenderLayer, "Upscale shaders not found in folder \"Content/Shaders/SpriteMasked/\", can't run layer!");
    }

    ShaderDescriptorLayout descriptorsLayout;
    descriptorsLayout.GenerateBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
    descriptorsLayout.GenerateBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

    shader = std::make_shared<IShader>(layerRef->GetParentLayer()->GetRenderPass(), VertShader->GetBuffer(), FragShader->GetBuffer(), descriptorsLayout);

    Resource *SpriteImage = AssetLoader::Get().LoadResource("Sprites/Test.png");

    int texWidth, texHeight, texChannels;
    unsigned char* pixels = stbi_load_from_memory(reinterpret_cast<stbi_uc*>(SpriteImage->GetBuffer().data()), SpriteImage->GetBuffer().size(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    // TODO: Finish it and refine
    texture = std::make_shared<ISamplerTexture>(texWidth, texHeight, pixels);

    material = std::make_shared<IMaterial>(shader.get());

    material->SetSampler(1, texture->GetImageView(), texture->GetSampler());
}

void SpriteRenderProxy::Render(VkCommandBuffer cmdBuffer, WorldRenderLayerRef* layerRef)
{
    IRenderUtility::BeginDebugLabel(cmdBuffer, "Sprite");

    material->SetBuffer(0, layerRef->GetSceneDataBuffer()->GetBufferObject(), layerRef->GetSceneDataBuffer()->GetSize());
    VkDescriptorSet set = material->Get(layerRef->GetParentComposition()->GetCurrentImageIndex());
    vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipelineLayout(), 0, 1, &set, 0, nullptr);

    vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipeline());

    vkCmdDraw(cmdBuffer, 3, 1, 0, 0);

    IRenderUtility::EndDebugLabel(cmdBuffer);
}