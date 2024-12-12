#include "WorldRenderLayer.h"
#include <Engine/EngineDelegates.h>
#include <functional>
#include <Log/Logger.h>
#include <RenderV/Utility.h>
#include <World/World.h>
#include <glm/ext.hpp>

#include <RenderV/Composition.h>

#include <Platform/Platform.h>

WorldRenderLayerRef::WorldRenderLayerRef()
    : world(nullptr)
{
    //EngineDelegates::OnWorldLoad.BindMember(this, &WorldRenderLayerRef::onWorldLoad);
}

void WorldRenderLayerRef::onWorldLoad(World* loadedWorld)
{
    world = loadedWorld;

    // Create Render Proxies and subscribe to world events
    if(!world){
        return;
    }

    world->OnEntitySpawned.BindMember(this, &WorldRenderLayerRef::onWorldEntitySpawned);

    for(Entity* entity : world->GetEntities()) {
        if(EntityRenderProxy* renderProxy = entity->CreateRenderProxy()) {
            renderProxy->CreateResources(this);
            renderProxies.push_back(renderProxy);
        }
    }
}

void WorldRenderLayerRef::onWorldEntitySpawned(Entity* entity)
{
    if(entity){
        if(EntityRenderProxy* renderProxy = entity->CreateRenderProxy()) {
            renderProxy->CreateResources(this);
            renderProxies.push_back(renderProxy);
        }
    }
}

WorldRenderLayerRef* WorldRenderLayerRef::SubscribeWorldLoad(MulticastDelegate<World*>* delegate)
{
    delegate->BindMember(this, &WorldRenderLayerRef::onWorldLoad);

    return this;
}

WorldRenderLayerRef* WorldRenderLayerRef::SubscribeWorldUnload(MulticastDelegate<World*>* delegate)
{
    delegate->BindMember(this, &WorldRenderLayerRef::onWorldUnload);

    return this;
}

bool WorldRenderLayerRef::Initialize(VkDevice device, RenderDependencyList<IRenderLayerRef>& DependencyList)
{
    VkPhysicalDevice physDevice = IRenderUtility::GetPhysicalDevice();

    const size_t framesInFlight = GetParentComposition()->GetFramesInFlight();

    // Create Swapchain
    // TODO: swapchain support info
    imageFormat = VK_FORMAT_B8G8R8A8_SRGB;

    pixelPerfectImages.resize(framesInFlight);
    pixelPerfectImageViews.resize(framesInFlight);
    pixelPerfectImageFramebuffers.resize(framesInFlight);
    pixelPerfectImageMemories.resize(framesInFlight);

    for(size_t i = 0; i < framesInFlight; i++) {
        VkImageCreateInfo imgInfo;
        imgInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imgInfo.flags = 0;
        imgInfo.pNext = nullptr;
        imgInfo.imageType = VK_IMAGE_TYPE_2D;
		imgInfo.format = imageFormat;
		imgInfo.extent.width = viewport.width;
		imgInfo.extent.height = viewport.height;
		imgInfo.extent.depth = 1;
		imgInfo.mipLevels = 1;
		imgInfo.arrayLayers = 1;
		imgInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imgInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imgInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        auto indices = IRenderUtility::FindQueueFamilies(IRenderUtility::GetPhysicalDevice());
        imgInfo.queueFamilyIndexCount = 1;
        imgInfo.pQueueFamilyIndices = &indices.graphicsFamily.value();
        imgInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imgInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(device, &imgInfo, nullptr, &pixelPerfectImages[i]) != VK_SUCCESS) {
            LOG(Fatal, LogWorldRenderLayer, "failed to create image!");
        }

        VkMemoryAllocateInfo memAlloc;
        memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memAlloc.pNext = nullptr;
        //memAlloc.allocationSize = ;
        VkMemoryRequirements memReqs;

        vkGetImageMemoryRequirements(device, pixelPerfectImages[i], &memReqs);
		memAlloc.allocationSize = memReqs.size;
		memAlloc.memoryTypeIndex = IRenderUtility::FindMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if(vkAllocateMemory(device, &memAlloc, nullptr, &pixelPerfectImageMemories[i]) != VK_SUCCESS) {
            LOG(Fatal, LogWorldRenderLayer, "failed to allocate device memory.");
        }

		if(vkBindImageMemory(device, pixelPerfectImages[i], pixelPerfectImageMemories[i], 0) != VK_SUCCESS) {
            LOG(Fatal, LogWorldRenderLayer, "failed to bind memory.");
        }

        VkImageViewCreateInfo viewInfo;
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.flags = 0;
        viewInfo.pNext = nullptr;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = imageFormat;
		viewInfo.subresourceRange = {};
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		viewInfo.image = pixelPerfectImages[i];

        viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        if (vkCreateImageView(device, &viewInfo, nullptr, &pixelPerfectImageViews[i]) != VK_SUCCESS) {
            LOG(Fatal, LogWorldRenderLayer, "failed to create image views.");
        }

        VkFramebufferCreateInfo framebufferInfo;
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.flags = 0;
        framebufferInfo.pNext = nullptr;
        framebufferInfo.width = viewport.width;
        framebufferInfo.height = viewport.height;
        framebufferInfo.renderPass = GetParentLayer()->GetRenderPass();
        framebufferInfo.layers = 1;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = &pixelPerfectImageViews[i];
       
        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &pixelPerfectImageFramebuffers[i]) != VK_SUCCESS) {
            LOG(Fatal, LogWorldRenderLayer,  "failed to create framebuffer!");
        }
    }

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_NEAREST;
    samplerInfo.minFilter = VK_FILTER_NEAREST;

    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 1.0f;
    
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(device, &samplerInfo, nullptr, &pixelPerfectSampler) != VK_SUCCESS) {
        LOG(Fatal, LogWorldRenderLayer,  "failed to create texture sampler!");
    }

    // Create GPU Data SSBO
    SceneDataStorages.resize(GetParentComposition()->GetFramesInFlight());
    for(const WorldRenderLayerGPUStorage& storage : SceneDataStorages)
    {
        SceneDataSSBOs.push_back(new Buffer(sizeof(WorldRenderLayerGPUStorage), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT));
    }

    return true;
}

bool WorldRenderLayerRef::Recreate() 
{
    // TODO
    // Recreate viewport framebuffers ?
    // Update GPU Data SSBO
    
    return true;
}

WorldRenderLayerRef* WorldRenderLayerRef::SetWorld(World* newWorld)
{
    world = newWorld;
    return this;
}

void WorldRenderLayer::CreateUpscalePipeline()
{
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport pipelineViewport{};
    pipelineViewport.x = 0.0f;
    pipelineViewport.y = 0.0f;
    pipelineViewport.width = 1280;
    pipelineViewport.height = 720;
    pipelineViewport.minDepth = 0.0f;
    pipelineViewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = {1280, 720};

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &pipelineViewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
    
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

    if (vkCreatePipelineLayout(IRenderUtility::GetDevice(), &pipelineLayoutInfo, nullptr, &upscalePipelineLayout) != VK_SUCCESS) {
        LOG(Fatal, LogRender, "failed to create pipeline layout!");
    }

    // TODO: Upscale Pipeline Stages
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 0;
    pipelineInfo.pStages = nullptr;

    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr; // Optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = upscalePipelineLayout;
    pipelineInfo.renderPass = upscaleRenderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional

    if (vkCreateGraphicsPipelines(IRenderUtility::GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &upscalePipeline) != VK_SUCCESS) {
        LOG(Fatal, LogRender, "failed to create pipeline!");
    }
}

void WorldRenderLayer::CreateUpscaleMaterial()
{
    //IFile* VertFile = IPlatform::Get()->OpenFile("./Shaders/TriangleUpscale/TriangleUpscale.vert.spv", FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY | FILE_ACCESS_FLAG_ATE);
    IFile* VertFile = IPlatform::Get()->OpenLocalFile("Editor/Shaders/TriangleUpscale/TriangleUpscale.vert.spv", FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY | FILE_ACCESS_FLAG_ATE);
    IFile* FragFile = IPlatform::Get()->OpenLocalFile("Editor/Shaders/TriangleUpscale/TriangleUpscale.frag.spv", FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY | FILE_ACCESS_FLAG_ATE);

    if(!VertFile->IsOpen() || !FragFile->IsOpen()) {
        LOG(Fatal, LogWorldRenderLayer, "Upscale shaders not found in folder \"Editor/Shaders/TriangleUpscale/\", can't run layer!");
    }

    ShaderDescriptorLayout descriptorsLayout;
    descriptorsLayout.GenerateBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

    upscaleShader = std::make_shared<IShader>(upscaleRenderPass, VertFile->FetchAllBinary(), FragFile->FetchAllBinary(), descriptorsLayout);

    if(!upscaleShader->IsValid())
    {
        LOG(Fatal, LogWorldRenderLayer, "Upscale shader creation failed");
    }

    upscaleMaterial = std::make_shared<IMaterial>(upscaleShader.get());
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
    //colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

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

    // Create Stretch Render Pass
    VkAttachmentDescription stretchColorAttachment{};
    stretchColorAttachment.format = VK_FORMAT_B8G8R8A8_SRGB;
    stretchColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    stretchColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    stretchColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    stretchColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    stretchColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    stretchColorAttachment.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    //stretchColorAttachment.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    stretchColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference stretchColorAttachmentRef{};
    stretchColorAttachmentRef.attachment = 0;
    stretchColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription stretchSubpass{};
    stretchSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    stretchSubpass.colorAttachmentCount = 1;
    stretchSubpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo stretchRenderPassInfo{};
    stretchRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    stretchRenderPassInfo.attachmentCount = 1;
    stretchRenderPassInfo.pAttachments = &stretchColorAttachment;
    stretchRenderPassInfo.subpassCount = 1;
    stretchRenderPassInfo.pSubpasses = &stretchSubpass;

    if (vkCreateRenderPass(device, &stretchRenderPassInfo, nullptr, &upscaleRenderPass) != VK_SUCCESS) {
        LOG(Fatal, LogRender, "failed to create render pass!");
    }

    //CreateUpscalePipeline();
    CreateUpscaleMaterial();

    return true;
}

void WorldRenderLayer::Prepare(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer)
{
    WorldRenderLayerRef* ref = ((WorldRenderLayerRef*)(layerRef));

    for(EntityRenderProxy* proxy : ref->renderProxies) {
        proxy->Update(ref);
    }

    // TODO: Update GPU data
    const size_t imageID = ref->GetParentComposition()->GetCurrentImageIndex();

    if(World* world = ref->GetWorld()) {
        ref->SceneDataStorages[imageID].ProjectionMatrix = glm::ortho(0.0f, static_cast<float>(ref->viewport.width), 0.0f, static_cast<float>(ref->viewport.height), -100.f, 100.0f);;
        ref->SceneDataStorages[imageID].ViewMatrix = glm::mat4(1);
    }
    
    /*for(int i = 0; i < ref->SceneDataSSBOs.size(); ++i)
    {
        ref->SceneDataSSBOs[i]->Write(&((WorldRenderLayerRef*)(layerRef))->SceneDataStorages[i]);
    }*/

   ref->SceneDataSSBOs[imageID]->Write(&ref->SceneDataStorages[imageID]);

    std::vector<VkImageView> upscaleViews = {
        ((WorldRenderLayerRef*)layerRef)->pixelPerfectImageViews[0],
        ((WorldRenderLayerRef*)layerRef)->pixelPerfectImageViews[1]
    };
    //upscaleMaterial->SetSamplerRenderTarget(0, upscaleViews, ((WorldRenderLayerRef*)layerRef)->pixelPerfectSampler);

    upscaleMaterial->SetSampler(0, ref->pixelPerfectImageViews[imageID], ((WorldRenderLayerRef*)layerRef)->pixelPerfectSampler);
}

void WorldRenderLayer::Render(VkCommandBuffer cmdBuffer, IRenderLayerRef* layerRef, IRenderLayerRef* previousLayer)
{
    if(((WorldRenderLayerRef*)layerRef)->world && 
        layerRef->GetParentComposition()->GetGameViewport().extent.width > 0 && 
        layerRef->GetParentComposition()->GetGameViewport().extent.height > 0)
    {
        uint32_t CurrentFrame = ((WorldRenderLayerRef*)layerRef)->GetParentComposition()->GetCurrentImageIndex(); 
        WorldRenderLayerRef* worldRef = (WorldRenderLayerRef*)layerRef;
        // TODO: Gradient
        const Vec3& WorldColorValue = worldRef->GetWorld()->BackgroundColor;

        IRenderUtility::BeginDebugLabel(cmdBuffer, "Pixel Perfect World", 0.35, 0.85, 0.5);
        
        IRenderUtility::BeginDebugLabel(cmdBuffer, "Original Viewport", 0.85, 0.85, 0.5);
        VkRenderPassBeginInfo passInfo;
        passInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        passInfo.pNext = nullptr;
        passInfo.renderPass = renderPass;
        VkClearValue clearVal = {{{WorldColorValue.x, WorldColorValue.y, WorldColorValue.z, 1}}};
        passInfo.pClearValues = &clearVal;
        passInfo.clearValueCount = 1;
        passInfo.framebuffer = worldRef->pixelPerfectImageFramebuffers[CurrentFrame]/*layerRef->GetParentComposition()->GetCurrentFramebuffer()*/;
        passInfo.renderArea.offset = {0, 0};
        passInfo.renderArea.extent = worldRef->viewport;

        vkCmdBeginRenderPass(cmdBuffer, &passInfo, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

        // Render
        VkViewport viewport;
        viewport.x = 0;
        viewport.y = 0;
        viewport.width = ((WorldRenderLayerRef*)layerRef)->viewport.width;
        viewport.height = ((WorldRenderLayerRef*)layerRef)->viewport.height;
        //viewport.width = layerRef->GetParentComposition()->GetExtent().width;
        //viewport.height = layerRef->GetParentComposition()->GetExtent().height;
        viewport.minDepth = 0;
        viewport.maxDepth = 1;
        
        VkRect2D scissors;
        scissors.offset = {0, 0};
        scissors.extent = ((WorldRenderLayerRef*)layerRef)->viewport;

        vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);
        vkCmdSetScissor(cmdBuffer, 0, 1, &scissors);

        for(EntityRenderProxy* proxy : ((WorldRenderLayerRef*)layerRef)->renderProxies)
        {
            if(proxy->Parent->Visible) 
            {
                proxy->Render(cmdBuffer, (WorldRenderLayerRef*)layerRef);
            }
        }

        for(EntityRenderProxy* postProxy : ((WorldRenderLayerRef*)layerRef)->postRenderProxies)
        {
            postProxy->Render(cmdBuffer, (WorldRenderLayerRef*)layerRef);
        }

        vkCmdEndRenderPass(cmdBuffer);
        IRenderUtility::EndDebugLabel(cmdBuffer);
        // Render End

        // Barrier
        /*VkImageMemoryBarrier imgMemBar = {};
        imgMemBar.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imgMemBar.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imgMemBar.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        imgMemBar.image = ((WorldRenderLayerRef*)layerRef)->pixelPerfectImages[CurrentFrame];
        imgMemBar.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imgMemBar.subresourceRange.baseMipLevel = 0;
        imgMemBar.subresourceRange.levelCount = 1;
        imgMemBar.subresourceRange.baseArrayLayer = 0;
        imgMemBar.subresourceRange.layerCount = 1;

        imgMemBar.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imgMemBar.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imgMemBar.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        imgMemBar.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    
        vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imgMemBar);*/

        IRenderUtility::ImageBarrier(cmdBuffer, ((WorldRenderLayerRef*)layerRef)->pixelPerfectImages[CurrentFrame], 
                                     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
                                     VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                     VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);

        IRenderUtility::BeginDebugLabel(cmdBuffer, "Upscale");
        // Stretch pass
        clearVal = {{{1, 0, 0, 1}}};
        passInfo.clearValueCount = 0;
        passInfo.renderPass = upscaleRenderPass;
        passInfo.framebuffer = layerRef->GetParentComposition()->GetCurrentFramebuffer();
        passInfo.renderArea.offset = layerRef->GetParentComposition()->GetOffset();
        passInfo.renderArea.extent = layerRef->GetParentComposition()->GetExtent();

        vkCmdBeginRenderPass(cmdBuffer, &passInfo, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

        const VkDescriptorSet upscaleDescSet = upscaleMaterial->Get(CurrentFrame);
        vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, upscaleShader->GetPipelineLayout(), 0, 1, &upscaleDescSet, 0, nullptr);

        vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, upscaleShader->GetPipeline());

        viewport.x = layerRef->GetParentComposition()->GetGameViewport().offset.x;
        viewport.y = layerRef->GetParentComposition()->GetGameViewport().offset.y;
        viewport.width = layerRef->GetParentComposition()->GetGameViewport().extent.width;
        viewport.height = layerRef->GetParentComposition()->GetGameViewport().extent.height;

        vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

        scissors.offset = layerRef->GetParentComposition()->GetGameViewport().offset;
        scissors.extent = layerRef->GetParentComposition()->GetGameViewport().extent;

        vkCmdSetScissor(cmdBuffer, 0, 1, &scissors);

        vkCmdDraw(cmdBuffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(cmdBuffer);
        IRenderUtility::EndDebugLabel(cmdBuffer);

        IRenderUtility::EndDebugLabel(cmdBuffer);
    }
}

WorldRenderLayerRef::~WorldRenderLayerRef()
{
    for(VkImage image : pixelPerfectImages){
        vkDestroyImage(IRenderUtility::GetDevice(), image, nullptr);
    }

    for(VkImageView view : pixelPerfectImageViews){
        vkDestroyImageView(IRenderUtility::GetDevice(), view, nullptr);
    }

    for(VkFramebuffer fb : pixelPerfectImageFramebuffers){
        vkDestroyFramebuffer(IRenderUtility::GetDevice(), fb, nullptr);
    }

    vkDestroySampler(IRenderUtility::GetDevice(), pixelPerfectSampler, nullptr);
}

bool WorldRenderLayer::Deinitialize()
{
    vkDestroyPipeline(IRenderUtility::GetDevice(), upscalePipeline, nullptr);
    vkDestroyPipelineLayout(IRenderUtility::GetDevice(), upscalePipelineLayout, nullptr);

    vkDestroyRenderPass(IRenderUtility::GetDevice(), renderPass, nullptr);
    vkDestroyRenderPass(IRenderUtility::GetDevice(), upscaleRenderPass, nullptr);

    return true;
}