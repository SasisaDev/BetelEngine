#include "WorldRenderLayer.h"
#include <Engine/EngineDelegates.h>
#include <functional>
#include <Log/Logger.h>
#include <RenderV/Utility.h>
#include <World/World.h>
#include <glm/ext.hpp>

#include <RenderV/Composition.h>

#include <Platform/Platform.h>

#include <AssetLoader/AssetLoader.h>

#ifdef EDITOR
#   include <Editor/Editor.h> 
#endif

WorldRenderLayerRef::WorldRenderLayerRef()
    : world(nullptr)
{
}

void WorldRenderLayerRef::CalculateAspectRatioCompensationData()
{
    float OriginalViewportAspect = ((float)viewport.width / (float)viewport.height);
    float CurrentViewportAspect =  (float)GetParentComposition()->GetGameViewport().extent.width / (float)GetParentComposition()->GetGameViewport().extent.height;
    
    if(CurrentViewportAspect > OriginalViewportAspect) {
        // Compensate for lack of Y
        UpscaleDataStorage.AspectRationCompensation.x = 1;
        // kY = Xc / ((Xo/Yo) * Yc)
        UpscaleDataStorage.AspectRationCompensation.y = (float)GetParentComposition()->GetGameViewport().extent.width / (OriginalViewportAspect * (float)GetParentComposition()->GetGameViewport().extent.height);
    } else if(CurrentViewportAspect < OriginalViewportAspect) {
        // Compensate for lack of X
        UpscaleDataStorage.AspectRationCompensation.y = 1;
        // kX = ((Xo/Yo) * Yc) / Xc
        UpscaleDataStorage.AspectRationCompensation.x = (OriginalViewportAspect * (float)GetParentComposition()->GetGameViewport().extent.height) / (float)GetParentComposition()->GetGameViewport().extent.width;
    } else {
        UpscaleDataStorage.AspectRationCompensation.y = 1;
        UpscaleDataStorage.AspectRationCompensation.x = 1;
    }

    UpscaleDataSSBO->Write(&UpscaleDataStorage);
}

void WorldRenderLayerRef::CreatePixelPerfectResources()
{
    VkDevice device = IRenderUtility::GetDevice();

    const size_t framesInFlight = GetParentComposition()->GetFramesInFlight();

    VkFormat depthImageFormat = IRenderUtility::FindDepthFormat();
    
    {
        VkImageCreateInfo depthImgInfo;
        depthImgInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        depthImgInfo.flags = 0;
        depthImgInfo.pNext = nullptr;
        depthImgInfo.imageType = VK_IMAGE_TYPE_2D;
		depthImgInfo.format = depthImageFormat;
		depthImgInfo.extent.width = viewport.width;
		depthImgInfo.extent.height = viewport.height;
		depthImgInfo.extent.depth = 1;
		depthImgInfo.mipLevels = 1;
		depthImgInfo.arrayLayers = 1;
		depthImgInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		depthImgInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		depthImgInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        auto indices = IRenderUtility::FindQueueFamilies(IRenderUtility::GetPhysicalDevice());
        depthImgInfo.queueFamilyIndexCount = 1;
        depthImgInfo.pQueueFamilyIndices = &indices.graphicsFamily.value();
        depthImgInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthImgInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(device, &depthImgInfo, nullptr, &pixelPerfectDepthImage) != VK_SUCCESS) {
            LOG(Fatal, LogWorldRenderLayer, "failed to create depth image!");
        }

        VkMemoryAllocateInfo memAlloc;
        memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memAlloc.pNext = nullptr;
        //memAlloc.allocationSize = ;
        VkMemoryRequirements memReqs;

        vkGetImageMemoryRequirements(device, pixelPerfectDepthImage, &memReqs);
		memAlloc.allocationSize = memReqs.size;
		memAlloc.memoryTypeIndex = IRenderUtility::FindMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if(vkAllocateMemory(device, &memAlloc, nullptr, &pixelPerfectDepthImageMemory) != VK_SUCCESS) {
            LOG(Fatal, LogWorldRenderLayer, "failed to allocate depth device memory.");
        }

		if(vkBindImageMemory(device, pixelPerfectDepthImage, pixelPerfectDepthImageMemory, 0) != VK_SUCCESS) {
            LOG(Fatal, LogWorldRenderLayer, "failed to bind depth memory.");
        }

        VkImageViewCreateInfo depthViewInfo;
        depthViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        depthViewInfo.flags = 0;
        depthViewInfo.pNext = nullptr;
        depthViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		depthViewInfo.format = depthImageFormat;
		depthViewInfo.subresourceRange = {};
		depthViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		depthViewInfo.subresourceRange.baseMipLevel = 0;
		depthViewInfo.subresourceRange.levelCount = 1;
		depthViewInfo.subresourceRange.baseArrayLayer = 0;
		depthViewInfo.subresourceRange.layerCount = 1;
		depthViewInfo.image = pixelPerfectDepthImage;

        depthViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        depthViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        depthViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        depthViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        if (vkCreateImageView(device, &depthViewInfo, nullptr, &pixelPerfectDepthImageView) != VK_SUCCESS) {
            LOG(Fatal, LogWorldRenderLayer, "failed to create depth image views.");
        }
    }

    // Create Stencil Swapchain
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
        std::vector<VkImageView> fbAttachments = {pixelPerfectImageViews[i], pixelPerfectDepthImageView};
        framebufferInfo.attachmentCount = static_cast<uint32_t>(fbAttachments.size());
        framebufferInfo.pAttachments = fbAttachments.data();
       
        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &pixelPerfectImageFramebuffers[i]) != VK_SUCCESS) {
            LOG(Fatal, LogWorldRenderLayer,  "failed to create framebuffer!");
        }
    }
}

void WorldRenderLayerRef::UpdateSceneView()
{
    if(!world) {
        return;
    }

    const float viewportWidth = static_cast<float>(viewport.width);
    const float viewportHeight = static_cast<float>(viewport.height);
    world->GetSceneView().UpdateProjectionMatrix(glm::ortho(-(viewportWidth/2), (viewportWidth/2), -(viewportHeight/2), (viewportHeight/2), -100.f, 100.0f));
    world->GetSceneView().UpdateViewMatrix(glm::mat4(1));
    world->GetSceneView().UpdateViewProjectionMatrix();
}

void WorldRenderLayerRef::Prepare(VkDevice device, IRenderLayerRef *previousLayer)
{
    if(bDirty) {
        UpdateSceneView();
    }
}

void WorldRenderLayerRef::onWorldLoad(World *loadedWorld)
{
    world = loadedWorld;

    // Create Render Proxies and subscribe to world events
    if(!world){
        return;
    }

    world->OnEntitySpawned.BindMember(this, &WorldRenderLayerRef::onWorldEntitySpawned);
    world->OnEntityDestroyed.BindMember(this, &WorldRenderLayerRef::onWorldEntityDespawned);

    for(const ObjectRef<Entity>& entity : world->GetEntities()) {
        if(EntityRenderProxy* renderProxy = entity.Get()->SetupRenderProxy(this)) {
            renderProxy->CreateResources(this);
            renderProxies.push_back(renderProxy);
        }
    }

    UpdateSceneView();
}

void WorldRenderLayerRef::onWorldUnload(World* loadedWorld)
{
    vkDeviceWaitIdle(IRenderUtility::GetDevice());

    // Remove all render proxies
    for(int i = 0; i < renderProxies.size(); ++i) {
        // FIXME: deleting any render proxy results in Vulkan errors
        delete renderProxies[i];
    }

    renderProxies.clear();
}

void WorldRenderLayerRef::onWorldEntitySpawned(Entity* entity)
{
    if(entity){
        if(EntityRenderProxy* renderProxy = entity->SetupRenderProxy(this)) {
            renderProxy->CreateResources(this);
            renderProxies.push_back(renderProxy);
        }
    }
}

void WorldRenderLayerRef::onWorldEntityDespawned(Entity *entity)
{
    if(entity){
        for(auto it = renderProxies.begin(); it != renderProxies.end(); ++it) {
            if((*it)->Parent->GetID() == entity->GetID()) {
                delete (*it);
                renderProxies.erase(it);
                break;
            }
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

    CreatePixelPerfectResources();

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
        SceneDataSSBOs.emplace_back(new Buffer(sizeof(WorldRenderLayerGPUStorage), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT));
    }

    // Create Upscale Data SSBO
    UpscaleDataSSBO = new Buffer(sizeof(UpscaleGPUStorage), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    UpscaleDataSSBO->Write(&UpscaleDataStorage);

    CalculateAspectRatioCompensationData();

    return true;
}

bool WorldRenderLayerRef::Recreate() 
{
    VkDevice device = IRenderUtility::GetDevice();

    vkDeviceWaitIdle(device);

    vkDestroyImage(device, pixelPerfectDepthImage, nullptr);
    vkDestroyImageView(device, pixelPerfectDepthImageView, nullptr);
    vkFreeMemory(device, pixelPerfectDepthImageMemory, nullptr);

    for(VkImage image : pixelPerfectImages){
        vkDestroyImage(device, image, nullptr);
    }

    for(VkImageView view : pixelPerfectImageViews){
        vkDestroyImageView(device, view, nullptr);
    }

    for(VkDeviceMemory mem : pixelPerfectImageMemories){
        vkFreeMemory(device, mem, nullptr);
    }

    for(VkFramebuffer fb : pixelPerfectImageFramebuffers){
        vkDestroyFramebuffer(device, fb, nullptr);
    }

    vkDeviceWaitIdle(device);

    // Recreate Framebuffers
    CreatePixelPerfectResources();

    // Update Upscale Buffer
    CalculateAspectRatioCompensationData();

    bDirty = false;
    
    return true;
}

WorldRenderLayerRef* WorldRenderLayerRef::SetWorld(World* newWorld)
{
    world = newWorld;
    return this;
}

WorldRenderLayerRef *WorldRenderLayerRef::SetViewportSize(VkExtent2D newViewport)
{
    viewport = newViewport; 

    // Update Projection matrix
    if(world){
        const float viewportWidth = static_cast<float>(viewport.width);
        const float viewportHeight = static_cast<float>(viewport.height);
        world->GetSceneView().UpdateProjectionMatrix(glm::ortho(-(viewportWidth/2), (viewportWidth/2), -(viewportHeight/2), (viewportHeight/2), -100.f, 100.0f));
    }

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
    //IFile* VertFile = IPlatform::Get()->OpenLocalFile("Shaders/TriangleUpscale/TriangleUpscale.vert.spv", FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY | FILE_ACCESS_FLAG_ATE);
    //IFile* FragFile = IPlatform::Get()->OpenLocalFile("Shaders/TriangleUpscale/TriangleUpscale.frag.spv", FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY | FILE_ACCESS_FLAG_ATE);

    Resource *VertShader = GEngine->GetAssetLoader()->LoadResource("Shaders/TriangleUpscale/TriangleUpscale.vert.spv");
    Resource *FragShader = GEngine->GetAssetLoader()->LoadResource("Shaders/TriangleUpscale/TriangleUpscale.frag.spv");

    if(VertShader == nullptr || FragShader == nullptr)
    {
        LOG(Fatal, LogWorldRenderLayer, "Upscale shaders not found in folder \"Editor/Shaders/TriangleUpscale/\", can't run layer!");
    }

    /*if(!VertFile->IsOpen() || !FragFile->IsOpen()) {
        LOG(Fatal, LogWorldRenderLayer, "Upscale shaders not found in folder \"Editor/Shaders/TriangleUpscale/\", can't run layer!");
    }*/

    ShaderDescriptorLayout descriptorsLayout;
    descriptorsLayout.GenerateBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
    descriptorsLayout.GenerateBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

    upscaleShader = std::make_unique<IShader>(upscaleRenderPass, VertShader->GetBuffer(), FragShader->GetBuffer(), descriptorsLayout);

    if(!upscaleShader->IsValid())
    {
        LOG(Fatal, LogWorldRenderLayer, "Upscale shader creation failed");
    }

    upscaleMaterial = std::make_unique<IMaterial>(upscaleShader.get());

    delete VertShader;
    delete FragShader;
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

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = IRenderUtility::FindDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //depthAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    std::vector<VkAttachmentDescription> attachments = {colorAttachment, depthAttachment};
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

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

    ref->Prepare(IRenderUtility::GetDevice(), previousLayer);

    for(EntityRenderProxy* proxy : ref->renderProxies) {
        proxy->Update(ref);
    }

    const size_t imageID = ref->GetParentComposition()->GetCurrentImageIndex();

    // TODO: Maybe we don't need to update this data each frame
    if(World* world = ref->GetWorld()) {
        ref->SceneDataStorages[imageID].ProjectionMatrix = world->GetSceneView().ProjectionMatrix;
        ref->SceneDataStorages[imageID].ViewMatrix = world->GetSceneView().ViewMatrix;
        ref->SceneDataStorages[imageID].CameraPosition = world->GetSceneView().ViewOrigin;
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
        const Vec3& WorldColorValue = worldRef->GetWorld()->BackgroundColor;

        IRenderUtility::BeginDebugLabel(cmdBuffer, "Pixel Perfect World", 0.35, 0.85, 0.5);
        
        IRenderUtility::BeginDebugLabel(cmdBuffer, "Original Viewport", 0.85, 0.85, 0.5);
        VkRenderPassBeginInfo passInfo;
        passInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        passInfo.pNext = nullptr;
        passInfo.renderPass = renderPass;
        VkClearValue clearVal = {{{WorldColorValue.x, WorldColorValue.y, WorldColorValue.z, 1}}};
        VkClearValue depthClearVal {};
        depthClearVal.depthStencil = {1.0f, 0};
        std::vector<VkClearValue> clearValues = {clearVal, depthClearVal};

        passInfo.pClearValues = clearValues.data();
        passInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        passInfo.framebuffer = worldRef->pixelPerfectImageFramebuffers[CurrentFrame];
        passInfo.renderArea.offset = {0, 0};
        passInfo.renderArea.extent = worldRef->viewport;

        vkCmdBeginRenderPass(cmdBuffer, &passInfo, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

        // Pixel Perfect World Render
        VkViewport viewport;
        viewport.x = 0;
        viewport.y = 0;
        viewport.width = ((WorldRenderLayerRef*)layerRef)->viewport.width;
        viewport.height = ((WorldRenderLayerRef*)layerRef)->viewport.height;
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

        // Render Post Render Proxies. These are mostly non-pixel perfect overlays of Editor Tools
        if(((WorldRenderLayerRef*)layerRef)->bRenderPostRenderProxies) {
            for(EntityRenderProxy* postProxy : ((WorldRenderLayerRef*)layerRef)->postRenderProxies)
            {
                postProxy->Render(cmdBuffer, (WorldRenderLayerRef*)layerRef);
            }
        }

        vkCmdEndRenderPass(cmdBuffer);
        IRenderUtility::EndDebugLabel(cmdBuffer);
        // Pixel Perfect World Render End

        IRenderUtility::ImageBarrier(cmdBuffer, ((WorldRenderLayerRef*)layerRef)->pixelPerfectImages[CurrentFrame], 
                                     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
                                     VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                     VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);

        // Upscale pass
        IRenderUtility::BeginDebugLabel(cmdBuffer, "Upscale");
        clearVal = {{{1, 0, 0, 1}}};
        passInfo.clearValueCount = 0;
        passInfo.renderPass = upscaleRenderPass;
        passInfo.framebuffer = layerRef->GetParentComposition()->GetCurrentFramebuffer();
        passInfo.renderArea.offset = layerRef->GetParentComposition()->GetOffset();
        passInfo.renderArea.extent = layerRef->GetParentComposition()->GetExtent();

        vkCmdBeginRenderPass(cmdBuffer, &passInfo, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

        upscaleMaterial->SetBuffer(1, ((WorldRenderLayerRef*)layerRef)->UpscaleDataSSBO->GetBufferObject(), ((WorldRenderLayerRef*)layerRef)->UpscaleDataSSBO->GetSize());
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
    for(auto proxy : renderProxies){
        delete proxy;
    }

    vkDestroyImage(IRenderUtility::GetDevice(), pixelPerfectDepthImage, nullptr);
    vkDestroyImageView(IRenderUtility::GetDevice(), pixelPerfectDepthImageView, nullptr);
    
    for(VkImage image : pixelPerfectImages){
        vkDestroyImage(IRenderUtility::GetDevice(), image, nullptr);
    }

    for(VkImageView view : pixelPerfectImageViews){
        vkDestroyImageView(IRenderUtility::GetDevice(), view, nullptr);
    }

    for(VkDeviceMemory mem : pixelPerfectImageMemories){
        vkFreeMemory(IRenderUtility::GetDevice(), mem, nullptr);
    }

    for(VkFramebuffer fb : pixelPerfectImageFramebuffers){
        vkDestroyFramebuffer(IRenderUtility::GetDevice(), fb, nullptr);
    }

    vkDestroySampler(IRenderUtility::GetDevice(), pixelPerfectSampler, nullptr);

    for(auto* buffer : SceneDataSSBOs) {
        delete buffer;
    }

    delete UpscaleDataSSBO;
}

bool WorldRenderLayer::Deinitialize()
{
    vkDestroyPipeline(IRenderUtility::GetDevice(), upscalePipeline, nullptr);
    vkDestroyPipelineLayout(IRenderUtility::GetDevice(), upscalePipelineLayout, nullptr);

    vkDestroyRenderPass(IRenderUtility::GetDevice(), upscaleRenderPass, nullptr);

    IRenderLayer::Deinitialize();

    return true;
}