#include "Engine.h"

#include <cassert>
#include <set>

#include <Log/Logger.h>

VkPhysicalDevice IRenderEngine::InternalPickPhysDevice(std::vector<VkPhysicalDevice> &PhysicalDevices)
{
    for(int i = 0; i < PhysicalDevices.size(); i++)
    {
        if(IRenderUtility::FindQueueFamilies(PhysicalDevices[i]).graphicsFamily.has_value())
        {
            return PhysicalDevices[i];
        }
    }

    LOG(Fatal, LogVulkan, "Found no physical devices supporting Vulkan on current system.");
    return nullptr;
}

bool IRenderEngine::Initialize(std::vector<const char*> extensions, std::vector<const char*> layers)
{
    // Initialize Vulkan Loader
    VkResult result = vkloader::InitializeLoader();
    if(result != VK_SUCCESS)
    {
        LOG(Fatal, LogVulkan, "Failed initializing vulkan loader");
        return false;
    }

    // Create Vulkan Instance
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = 0;
    appInfo.pEngineName = "Konide";
    appInfo.pApplicationName = "Arteria";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo instanceInfo = {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pNext = 0;
    instanceInfo.pApplicationInfo = &appInfo;

    std::vector<const char*> exts/* = InternalAssembleExtensions()*/;
    exts.insert(exts.end(), extensions.begin(), extensions.end());

    instanceInfo.enabledExtensionCount = exts.size();
    instanceInfo.ppEnabledExtensionNames = exts.data();

    std::vector<const char*> layrs/* = InternalAssembleLayers()*/;
    layrs.insert(layrs.end(), layers.begin(), layers.end());

    instanceInfo.enabledLayerCount = layrs.size();
    instanceInfo.ppEnabledLayerNames = layrs.data();

    result = vkloader::vkCreateInstance(&instanceInfo, nullptr, &instance);
    if(result != VK_SUCCESS)
    {
        LOG(Fatal, LogVulkan, "Failed creating VkInstance");
        return false;
    }

    // Pick Physical Device
    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        LOG(Fatal, LogVulkan, "failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    physDevice = DelegatePickPhysDevice(devices);

    vkloader::SetPhysicalDevice(physDevice);

    return true;
}

void IRenderEngine::InitializeWithSurface(VkSurfaceKHR surface)
{
    queueFamilyIndices = IRenderUtility::FindQueueFamilies(physDevice, surface);
}

void IRenderEngine::CreateDevice(std::vector<const char*> devExtensions, std::vector<const char*> devLayers)
{
    // Ensure all family indices are picked
    if(!queueFamilyIndices.presentFamily.has_value())
    {
        LOG(Warning, LogVulkan, "Presentation family index was not picked before device creation. Presentation may be unavailable. Are there any Surface Compositions?");
        queueFamilyIndices.presentFamily = queueFamilyIndices.graphicsFamily;
    }

    if(!queueFamilyIndices.computeFamily.has_value())
    {
        LOG(Warning, LogVulkan, "Presentation family index was not picked before device creation. Compute shaders would not be unavailable.");
    }

    // Create Device Queue
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value(), queueFamilyIndices.computeFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.pNext = nullptr;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    // Create Device
    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    // TODO: automatic extension list generator
    std::vector<const char*> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    extensions.insert(extensions.end(), devExtensions.begin(), devExtensions.end());
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = extensions.data();

    std::vector<const char*> layers = {};
    layers.insert(layers.end(), devLayers.begin(), devLayers.end());
    deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
    deviceCreateInfo.ppEnabledLayerNames = layers.data();

    if (vkloader::vkCreateDevice(physDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
        LOG(Fatal, LogVulkan, "Failed creating device");
    }

    vkGetDeviceQueue(device, queueFamilyIndices.graphicsFamily.value(), 0, &graphicsQueue);
    if(queueFamilyIndices.presentFamily.has_value()) 
    {   
        vkGetDeviceQueue(device, queueFamilyIndices.presentFamily.value(), 0, &presentQueue);
    }
    if(queueFamilyIndices.computeFamily.has_value()) 
    {   
        vkGetDeviceQueue(device, queueFamilyIndices.computeFamily.value(), 0, &computeQueue);
    }

    // Create synchronization objects
    VkSemaphoreCreateInfo semaphoreCreateInfo;
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.flags = 0;
    semaphoreCreateInfo.pNext = nullptr;

    vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &aquireSemaphore);
    vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &submitSemaphore);

    // Create command pool
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    
    vkCreateCommandPool(device, &poolInfo, nullptr, &cmdPool);
}

IRenderLayer* IRenderEngine::GetLayer(uint32_t id)
{
    return Layers[id];
}

uint32_t IRenderEngine::CreateComposition(IRenderCompositionInitializer* initializer)
{
    IRenderComposition* composition = new IRenderComposition();
    composition->Initialize(initializer);
    
    /*if(!queueFamilyIndices.IsComplete())
    {
        queueFamilyIndices = IRenderUtility::FindQueueFamilies(physDevice, composition->GetSurface());
    }*/

    Compositions.push_back(composition);
    return Compositions.size()-1;
}

IRenderComposition* IRenderEngine::GetComposition(uint32_t id)
{
    return Compositions[id];
}

void IRenderEngine::Render()
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandBufferCount = 1;
    allocInfo.commandPool = cmdPool;
    vkAllocateCommandBuffers(device, &allocInfo, &cmdBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cmdBuffer, &beginInfo);

    std::vector<VkSwapchainKHR> swapchains;
    std::vector<uint32_t> swapchainImageIndices;

    for(size_t compositionId = 0; compositionId < Compositions.size(); ++compositionId) {
        IRenderComposition* composition = Compositions[compositionId];

        composition->StartFrame(cmdBuffer);
        
        if(composition->GetType() == ERenderCompositionType::RENDER_COMPOSITION_TYPE_SURFACE) {
            swapchains.push_back(composition->GetSwapchain());
            swapchainImageIndices.push_back(composition->GetCurrentImageIndex());
        } else {

        }

        Compositions[compositionId]->Render(cmdBuffer);

        composition->EndFrame(cmdBuffer);
    }

    vkEndCommandBuffer(cmdBuffer);

    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pWaitDstStageMask = &waitStage;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuffer;
    submitInfo.pSignalSemaphores = &submitSemaphore;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &aquireSemaphore;
    submitInfo.waitSemaphoreCount = 1;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, 0);
    
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pSwapchains = swapchains.data();
    presentInfo.swapchainCount = 1;
    presentInfo.pImageIndices = swapchainImageIndices.data();
    presentInfo.pWaitSemaphores = &submitSemaphore;
    presentInfo.waitSemaphoreCount = 1;

    vkQueuePresentKHR(presentQueue, &presentInfo);

    vkDeviceWaitIdle(device);

    // Frame Cleanup
    vkFreeCommandBuffers(device, cmdPool, 1, &cmdBuffer);
}

// Debug implementations
#if !defined(NDEBUG)
void IRenderEngine::CreateDebugMessenger(PFN_vkDebugUtilsMessengerCallbackEXT callback, void* userData)
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = callback;
    createInfo.pUserData = userData;

    vkCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);
}
#endif