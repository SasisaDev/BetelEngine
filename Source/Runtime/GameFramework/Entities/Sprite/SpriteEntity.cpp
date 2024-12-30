#include "SpriteEntity.h"
#include <World/RenderLayer/WorldRenderLayer.h>
#include <stb/stb_image.h>

Buffer* SpriteRenderProxy::vertexBuffer = nullptr;
Buffer* SpriteRenderProxy::indexBuffer = nullptr;

void SpriteRenderProxy::CreateResources(WorldRenderLayerRef* layerRef)
{
    // Create Static Resources
    if (SpriteRenderProxy::vertexBuffer == nullptr || SpriteRenderProxy::indexBuffer == nullptr) 
    {
        Vertex vertices[4] = {{{-0.0f, 0.0f},  {0.0f, 0.0f}},
                              {{1.0f, 0.0f},   {1.0f, 0.0f}},
                              {{1.0f, 1.0f},    {1.0f, 1.0f}},
                              {{-0.0f, 1.0f},   {0.0f, 1.0f}}};
        SpriteRenderProxy::vertexBuffer = new Buffer(sizeof(vertices), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertices);

        uint16_t indices[6] = {0, 1, 2, 2, 3, 0};
        SpriteRenderProxy::indexBuffer = new Buffer(sizeof(indices), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indices);
    }

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
    descriptorsLayout.SetPushConstantRange(sizeof(SpriteEntityPushConstants));

    ShaderCreateInfo sCreateInfo = {};

    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    sCreateInfo.VertexInputBindingDescriptions = {bindingDescription};

    VkVertexInputAttributeDescription positionAttrPos = {};
    positionAttrPos.binding = 0;
    positionAttrPos.location = 0;
    positionAttrPos.format = VK_FORMAT_R32G32_SFLOAT;
    positionAttrPos.offset = offsetof(Vertex, position);
    VkVertexInputAttributeDescription positionAttrUV = {};
    positionAttrUV.binding = 0;
    positionAttrUV.location = 1;
    positionAttrUV.format = VK_FORMAT_R32G32_SFLOAT;
    positionAttrUV.offset = offsetof(Vertex, uv);
    sCreateInfo.VertexInputAtrributeDescriptions = {positionAttrPos, positionAttrUV};

    sCreateInfo.Blending.Enabled = true;
    sCreateInfo.Blending.WriteAlpha = false;

    shader = std::make_shared<IShader>(layerRef->GetParentLayer()->GetRenderPass(), VertShader->GetBuffer(), FragShader->GetBuffer(), descriptorsLayout, sCreateInfo);

    Resource *SpriteImage = AssetLoader::Get().LoadResource("Sprites/TestSemi.png");

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
    
    VkBuffer vertBuffers[1] = {SpriteRenderProxy::vertexBuffer->GetBufferObject()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(cmdBuffer, 0, 1, vertBuffers, offsets);
    vkCmdBindIndexBuffer(cmdBuffer, SpriteRenderProxy::indexBuffer->GetBufferObject(), 0, VK_INDEX_TYPE_UINT16);

    constants.Position = glm::vec2(Parent->GetLocation().x, Parent->GetLocation().y);
    // TODO: Render data for sprite must come from Sprite System 
    constants.Size = glm::vec2(texture->GetWidth(), texture->GetHeight());

    vkCmdPushConstants(cmdBuffer, material->GetShader()->GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(SpriteEntityPushConstants), &constants);

    vkCmdDrawIndexed(cmdBuffer, 6, 1, 0, 0, 0);

    IRenderUtility::EndDebugLabel(cmdBuffer);
}

void SpriteEntity::Tick(float deltaTime)
{
    // TODO: Remove experiment code
    static float param = 0;
    param += deltaTime;
    double sin = glm::sin(param / 1.0) * 100.0;
    double cos = glm::cos(param / 1.5) * 100.0;
    SetRelativeLocation({static_cast<int>(sin) , static_cast<int>(cos) , 0});
}