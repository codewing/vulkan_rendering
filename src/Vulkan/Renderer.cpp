//
// Created by codewing on 20/10/2017.
//

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>
#include <set>
#include <algorithm>

#include "../BUILD_OPTIONS.h"
#include "vkr/Image.h"
#include "vkr/UniformBufferObject.h"
#include "vkr/Utilities.h"
#include "vkr/Vertex.h"
#include "vkr/Window.h"
#include "vkr/Mesh.h"

#include "vkr/Vulkan/Renderer.h"
#include "QueueFamilyIndices.h"
#include "VulkanMemory.h"
#include "VulkanDevice.h"
#include "VulkanCommand.h"
#include "Image/VulkanImage.h"
#include "Image/VulkanSampler.h"
#include "VulkanUtilities.h"
#include "Descriptor/DescriptorSetLayoutBinding.h"
#include "Descriptor/DescriptorSetLayout.h"
#include "Descriptor/DescriptorPool.h"

Renderer::Renderer(std::shared_ptr<Window> window) : window(window) {
    window->OnResizedEvent = [&]() { RecreateSwapchain(); };
    InitVulkan();
}

Renderer::~Renderer() {
    DeInitVulkan();
}

void Renderer::SetupScene(std::shared_ptr<Scene> scene) {
    currentScene = scene;

    SetupMeshes();

    CreateCommandBuffers();
    CreateSemaphores();
}

void Renderer::SetupMeshes() {
    for (auto& mesh : currentScene->GetMeshes()) {
        mesh->CreateBuffers(*this);

        mesh->CreateTexture(*this);
        mesh->CreateSampler(*this);
        mesh->CreateDescriptors(*this);

        mesh->CopyDataToGPU(*this);

        if(graphicsPipeline == nullptr) CreateGraphicsPipeline(mesh->GetDescriptorSetLayout());
    }
}

void Renderer::InitVulkan() {
    CreateInstance();
    SetupDebugCallbacks();
    CreateSurface();
    SetupPhysicalDevice();
    InitLogicalDevice();
    CreateSwapchain();
    CreateImageViews();
    CreateRenderPass();
    CreateCommandPools();
    CreateDepthResources();
    CreateFramebuffers();
}

void Renderer::DeInitVulkan() {
    CleanupSwapchain();

    for(auto& mesh : currentScene->GetMeshes()) {
        mesh->FreeMesh(*this);
    }

    DestroySemaphores();
    DestroyCommandPools();

    DeInitLogicalDevice();
    DestroyDebugReportCallbackEXT(instance, callback, nullptr);
    DestroySurface();
    DestroyInstance();
}

void Renderer:: CreateInstance() {

#ifdef BUILD_ENABLE_VULKAN_DEBUG
    if (!CheckAllValidationLayersSupported()) {
        throw std::runtime_error("more validation layers requested than available");
    }
#endif

    VkApplicationInfo applicationInfo{};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = "Vulkan Rendering";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = "No Engine";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;

#ifdef BUILD_ENABLE_VULKAN_DEBUG
    instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
#else
    instanceCreateInfo.enabledLayerCount = 0;
#endif

    auto extensions = GetRequiredExtensions();
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

    ErrorCheck(vkCreateInstance(&instanceCreateInfo, nullptr, &instance));
}

void Renderer::DestroyInstance() {
    vkDestroyInstance(instance, nullptr);
}

bool Renderer::CheckAllValidationLayersSupported() {
    // Get all available layers
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayerProperties(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayerProperties.data());

    // check if all required layers are supported
    for (const char *layerName : validationLayers) {
        bool layerFound = false;

        for (const auto &layerProperties : availableLayerProperties) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

std::vector<const char *> Renderer::GetRequiredExtensions() {
    std::vector<const char *> extensions;

    unsigned int glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (unsigned int i = 0; i < glfwExtensionCount; i++) {
        extensions.push_back(glfwExtensions[i]);
    }

#if BUILD_ENABLE_VULKAN_DEBUG
    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif

    return extensions;
}

VkBool32 VKAPI_CALL Renderer::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
                                            uint64_t obj, size_t location, int32_t code, const char *layerPrefix,
                                            const char *msg, void *userData) {
    std::cerr << "validation layer: " << msg << std::endl;

    return VK_FALSE;
}

void Renderer::SetupDebugCallbacks() {
#if BUILD_ENABLE_VULKAN_DEBUG
    VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfoEXT{};
    debugReportCallbackCreateInfoEXT.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    debugReportCallbackCreateInfoEXT.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    debugReportCallbackCreateInfoEXT.pfnCallback = debugCallback;

    if (CreateDebugReportCallbackEXT(instance, &debugReportCallbackCreateInfoEXT, nullptr, &callback) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug callback!");
    }
#endif
}

VkResult
Renderer::CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
                                       const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback) {
    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void Renderer::DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback,
                                             const VkAllocationCallbacks *pAllocator) {
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance,
                                                                            "vkDestroyDebugReportCallbackEXT");
    if (func != nullptr) {
        func(instance, callback, pAllocator);
    }
}

bool Renderer::Run() {
    if (window != nullptr) {
        bool running = window->Update();

        DrawFrame();

        return running;
    }
    return false;
}

void Renderer::SetupPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

    for (const auto &currentPhysical : physicalDevices) {
        if (IsDeviceSuitable(currentPhysical)) {
            physicalDevice = currentPhysical;
            vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

bool Renderer::IsDeviceSuitable(VkPhysicalDevice vkPhysicalDevice) {
    QueueFamilyIndices indices = VulkanDevice::FindQueueFamilies(vkPhysicalDevice, surface,
                                                                 QueueFamilyType::GRAPHICS_WITH_PRESENT_FAMILY);

    bool extensionsSupported = CheckDeviceExtensionSupport(vkPhysicalDevice);

    bool swapChainSupported = false;
    // the swapChain requires extensions
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainDetails = QuerySwapchainSupport(vkPhysicalDevice);
        swapChainSupported = !swapChainDetails.formats.empty() && !swapChainDetails.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &supportedFeatures);

    return indices.isGraphicsWithPresentFamilySet() && swapChainSupported && supportedFeatures.samplerAnisotropy;
}

void Renderer::InitLogicalDevice() {
    QueueFamilyIndices graphicsFamilyIndices = VulkanDevice::FindQueueFamilies(physicalDevice, surface,
                                                                               QueueFamilyType::GRAPHICS_WITH_PRESENT_FAMILY);
    QueueFamilyIndices transferFamilyIndices = VulkanDevice::FindQueueFamilies(physicalDevice, surface,
                                                                               QueueFamilyType::TRANSFER_FAMILY);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfoVector;
    std::set<uint32_t> uniqueQueueFamilies = {graphicsFamilyIndices.GetGraphicsFamily(),
                                              graphicsFamilyIndices.GetPresentFamily(),
                                              transferFamilyIndices.GetTransferFamily()};

    float queuePriority = 1.0f;

    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfoVector.push_back(queueCreateInfo);
    }


    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfoVector.data();
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfoVector.size());
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

#if BUILD_ENABLE_VULKAN_DEBUG
    deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
#else
    deviceCreateInfo.enabledLayerCount = 0;
#endif

    ErrorCheck(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device));

    vkGetDeviceQueue(device, graphicsFamilyIndices.GetGraphicsFamily(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, graphicsFamilyIndices.GetPresentFamily(), 0, &presentQueue);
    vkGetDeviceQueue(device, transferFamilyIndices.GetTransferFamily(), 0, &transferQueue);
}

void Renderer::DeInitLogicalDevice() {
    vkDestroyDevice(device, nullptr);
}

void Renderer::CreateSurface() {
    window->CreateSurface(instance, &surface);
}

void Renderer::DestroySurface() {
    vkDestroySurfaceKHR(instance, surface, nullptr);
}

void Renderer::CreateSwapchain() {
    SwapChainSupportDetails swapchainDetails = QuerySwapchainSupport(physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapchainSurfaceFormat(swapchainDetails.formats);
    VkPresentModeKHR presentMode = ChooseSwapchainPresentMode(swapchainDetails.presentModes);
    VkExtent2D extent = ChooseSwapchainExtent(swapchainDetails.capabilities);

    // number of swap chain images
    uint32_t imageCount = swapchainDetails.capabilities.minImageCount + 1;
    if (swapchainDetails.capabilities.maxImageCount > 0 && imageCount > swapchainDetails.capabilities.maxImageCount) {
        imageCount = swapchainDetails.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapchainCreateInfo{};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.surface = surface;
    swapchainCreateInfo.minImageCount = imageCount;
    swapchainCreateInfo.imageFormat = surfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapchainCreateInfo.imageExtent = extent;
    swapchainCreateInfo.imageArrayLayers = 1;
    // render directly to image - VK_IMAGE_USAGE_TRANSFER_DST_BIT for post processing or other operations before rendering
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indicesGraphics = VulkanDevice::FindQueueFamilies(physicalDevice, surface,
                                                                         QueueFamilyType::GRAPHICS_WITH_PRESENT_FAMILY);
    QueueFamilyIndices indicesTransfer = VulkanDevice::FindQueueFamilies(physicalDevice, surface,
                                                                         QueueFamilyType::TRANSFER_FAMILY);

    std::vector<uint32_t> queueFamilyIndices = {indicesGraphics.GetGraphicsFamily(),
                                                indicesTransfer.GetTransferFamily()};

    if (indicesGraphics.GetGraphicsFamily() != indicesGraphics.GetPresentFamily()) {
        queueFamilyIndices.push_back(indicesGraphics.GetPresentFamily());
    }

    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    swapchainCreateInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
    swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();

    swapchainCreateInfo.preTransform = swapchainDetails.capabilities.currentTransform;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // Don't blend with other windows
    swapchainCreateInfo.presentMode = presentMode;
    swapchainCreateInfo.clipped = VK_TRUE;
    swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE; // Used for swapchain re-creation

    ErrorCheck(vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain));

    GetSwapchainImages();

    // save the format and the extent
    swapchainImageFormat = surfaceFormat.format;
    swapchainExtent = extent;
}

void Renderer::DestroySwapchain() {
    vkDestroySwapchainKHR(device, swapchain, nullptr);
}

void Renderer::RecreateSwapchain() {
    int width = window->Width();
    int height = window->Height();

    if (width == 0 || height == 0) return;

    vkDeviceWaitIdle(device);

    CleanupSwapchain();

    CreateSwapchain();
    CreateImageViews();
    CreateRenderPass();

    CreateDepthResources();
    CreateFramebuffers();

    SetupMeshes();

    CreateCommandBuffers();
}

void Renderer::CleanupSwapchain() {
    DestroyDepthResources();

    DestroyFramebuffers();
    vkFreeCommandBuffers(device, graphicCommandPool, static_cast<uint32_t>(graphicCommandBuffers.size()),
                         graphicCommandBuffers.data());
    DestroyGraphicsPipeline();
    DestroyRenderPass();
    DestroyImageViews();
    DestroySwapchain();

    for(auto& mesh : currentScene->GetMeshes()) {
        mesh->DestroyUniformBuffers(*this);
        mesh->DestroyDescriptors(*this);
    }
}

void Renderer::GetSwapchainImages() {
    uint32_t imageCount;
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());
}

void Renderer::CreateImageViews() {
    swapchainImageViews.resize(swapchainImages.size());

    for (size_t i = 0; i < swapchainImages.size(); i++) {
        swapchainImageViews[i] = VulkanImage::CreateImageViewForImage(device, swapchainImages[i], swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

void Renderer::DestroyImageViews() {
    for (auto imageView : swapchainImageViews) {
        vkDestroyImageView(device, imageView, nullptr);
    }
}

void Renderer::CreateGraphicsPipeline(std::shared_ptr<DescriptorSetLayout> descriptorSetLayout) {
    pipelineLayout = std::make_shared<PipelineLayout>();
    pipelineLayout->descriptorSetLayouts.push_back(descriptorSetLayout);

    graphicsPipeline = std::make_shared<Pipeline>(pipelineLayout);

    std::shared_ptr<ShaderModule> vertexShaderModule = std::make_shared<ShaderModule>("assets/shaders/shader.vert.spv");
    std::shared_ptr<ShaderModule> fragmentShaderModule = std::make_shared<ShaderModule>("assets/shaders/shader.frag.spv");

    graphicsPipeline->shaderStages =  {
            {VK_SHADER_STAGE_VERTEX_BIT, vertexShaderModule},
            {VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShaderModule}
    };

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(window->Width());
    viewport.height = static_cast<float>(window->Height());
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    graphicsPipeline->viewports = { viewport };

    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    //use the whole framebuffer
    scissor.extent = {static_cast<uint32_t>(window->Width()), static_cast<uint32_t>(window->Height())};

    graphicsPipeline->scissors = { scissor };

    RenderContext context = {};
    context.device = device;
    context.renderPass = renderPass;

    graphicsPipeline->Compile(context);
}

void Renderer::DestroyGraphicsPipeline() {
    pipelineLayout->FreePipelineLayout();
    graphicsPipeline.reset();
}

void Renderer::CreateRenderPass() {
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = swapchainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    // stencil is unused so don't care
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // image in the swapchain

    VkAttachmentDescription depthAttachment = {};
    depthAttachment.format = FindDepthBufferFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // Subpasses
    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0; // fragment shader out parameter
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef = {};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    ErrorCheck(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass));
}

void Renderer::DestroyRenderPass() {
    vkDestroyRenderPass(device, renderPass, nullptr);
}

void Renderer::CreateFramebuffers() {
    swapchainFramebuffers.resize(swapchainImageViews.size());

    for (size_t i = 0; i < swapchainImageViews.size(); i++) {
        std::array<VkImageView, 2> attachments = {
            swapchainImageViews[i],
            depthImage->GetImageView()
        };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapchainExtent.width;
        framebufferInfo.height = swapchainExtent.height;
        framebufferInfo.layers = 1;

        ErrorCheck(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapchainFramebuffers[i]));
    }
}

void Renderer::DestroyFramebuffers() {
    for (auto frameBuffer : swapchainFramebuffers) {
        vkDestroyFramebuffer(device, frameBuffer, nullptr);
    }
}

void Renderer::CreateCommandPools() {
    VulkanCommand::CreateCommandPool(device, physicalDevice, surface, QueueFamilyType::GRAPHICS_WITH_PRESENT_FAMILY,
                                     &graphicCommandPool);
    VulkanCommand::CreateCommandPool(device, physicalDevice, surface, QueueFamilyType::TRANSFER_FAMILY,
                                     &transferCommandPool);
}

void Renderer::CreateMeshBuffer(VkDeviceSize size, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VulkanMemory::CreateBufferAndBindMemory(device, physicalDevice, size, 
                                            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                            buffer, bufferMemory);
}

void Renderer::CreateUBOBuffer(VkDeviceSize size, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VulkanMemory::CreateBufferAndBindMemory(device, physicalDevice, size,VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                            buffer, bufferMemory);
}

void Renderer::CopyDataToBuffer(void* data, VkDeviceSize dataSize, VkBuffer buffer, VkDeviceSize bufferOffset) {
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    // Create staging buffer + memory
    VulkanMemory::CreateBufferAndBindMemory(device, physicalDevice, dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    VulkanMemory::CopyMemoryToGpu(device, stagingBufferMemory, data, dataSize);

    VulkanCommand::CopyBuffer(device, transferCommandPool, stagingBuffer, buffer, dataSize, transferQueue, bufferOffset);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Renderer::DestroyBuffer(VkBuffer buffer, VkDeviceMemory bufferMemory) {
    vkDestroyBuffer(device, buffer, nullptr);
    vkFreeMemory(device, bufferMemory, nullptr);
}

void Renderer::UpdateUniformBuffer(VkDeviceMemory uniformBufferMemory, UniformBufferObject& ubo, VkDeviceSize bufferOffset) {
    VulkanMemory::CopyMemoryToGpu(device, uniformBufferMemory, &ubo, sizeof(UniformBufferObject), bufferOffset);
}

void Renderer::CreateCommandBuffers() {
    graphicCommandBuffers.resize(swapchainFramebuffers.size());

    VkCommandBufferAllocateInfo commandBufferAllocInfo{};
    commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocInfo.commandPool = graphicCommandPool;
    commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocInfo.commandBufferCount = static_cast<uint32_t>(graphicCommandBuffers.size());

    ErrorCheck(vkAllocateCommandBuffers(device, &commandBufferAllocInfo, graphicCommandBuffers.data()));

    for (size_t i = 0; i < graphicCommandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        beginInfo.pInheritanceInfo = nullptr; // Optional

        vkBeginCommandBuffer(graphicCommandBuffers[i], &beginInfo);

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = swapchainFramebuffers[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapchainExtent;

        std::array<VkClearValue, 2> clearValues = {};
        // order = order of attachments in the render pass
        clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f}; // black bg
        clearValues[1].depthStencil = {1.0f, 0}; // 1 far -> 0 near plane

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(graphicCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(graphicCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->Handle());

        for (const auto& mesh : currentScene->GetMeshes() ) {
            VkBuffer vertexBuffers[] = { mesh->GetBufferHandle() };
            VkDeviceSize offsets[] = { mesh->GetVertexBufferOffset() };

            vkCmdBindVertexBuffers(graphicCommandBuffers[i], 0, 1, vertexBuffers, offsets);

            vkCmdBindIndexBuffer(graphicCommandBuffers[i], mesh->GetBufferHandle(), mesh->GetIndexBufferOffset(), VK_INDEX_TYPE_UINT32);

            VkDescriptorSet descriptorSet = mesh->GetDescriptorSet(i);
            vkCmdBindDescriptorSets(graphicCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout->Handle(), 0, 1,
                                    &descriptorSet, 0, nullptr);

            vkCmdDrawIndexed(graphicCommandBuffers[i], static_cast<uint32_t>(mesh->GetIndices().size()), 1, 0, 0, 0);

        }

        vkCmdEndRenderPass(graphicCommandBuffers[i]);

        ErrorCheck(vkEndCommandBuffer(graphicCommandBuffers[i]));
    }
}

void Renderer::DestroyCommandPools() {
    vkDestroyCommandPool(device, graphicCommandPool, nullptr);
    vkDestroyCommandPool(device, transferCommandPool, nullptr);
}

void Renderer::DrawFrame() {
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, swapchain, std::numeric_limits<uint64_t>::max(),
                                            imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
    // check whether the swapchain has changed
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        RecreateSwapchain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        ErrorCheck(result);
    }

    for(auto& mesh : currentScene->GetMeshes()) {
        mesh->UpdateUniformBuffer(*this, imageIndex);
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &graphicCommandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    ErrorCheck(vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE));

    // Presentation
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional

    result = vkQueuePresentKHR(presentQueue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        RecreateSwapchain();
    } else {
        ErrorCheck(result);
    }

    vkQueueWaitIdle(presentQueue);

}

void Renderer::CreateSemaphores() {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    ErrorCheck(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore));
    ErrorCheck(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore));
}

void Renderer::DestroySemaphores() {
    vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
}

/// check whether the required extensions are present
bool Renderer::CheckDeviceExtensionSupport(VkPhysicalDevice vkPhysicalDevice) {

    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto &extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

/// Get the swapchain info
SwapChainSupportDetails Renderer::QuerySwapchainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

/// Color format of the surface
VkSurfaceFormatKHR Renderer::ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
    // No preference?
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
        return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    // Our preference
    for (const auto &availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    // Otherwise the first should be fine
    return availableFormats[0];
}

/// In which order will the image be presented? Fifo -> Add image to queue | Mailbox Triple Buffer with replace | Immediate
VkPresentModeKHR Renderer::ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
    VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

    for (const auto &availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        } else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            bestMode = availablePresentMode;
        }
    }

    return bestMode;
}

/// Size of the rendered image
VkExtent2D Renderer::ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width = window->Width();
        int height = window->Height();

        VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                                   static_cast<uint32_t>(height)};

        actualExtent.width = std::max(capabilities.minImageExtent.width,
                                      std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height,
                                       std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

VkFormat Renderer::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        VkFormatProperties properties;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &properties);

        if(tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features) {
            return format;
        }

        if(tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features)  == features) {
            return format;
        }
    }

    throw std::runtime_error("failed to find a supported format!");
}

VkFormat Renderer::FindDepthBufferFormat() {
    return FindSupportedFormat( {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, 
                                VK_IMAGE_TILING_OPTIMAL, 
                                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT );
}

bool Renderer::HasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void Renderer::CreateDescriptors(Mesh& mesh) {
    std::vector<DescriptorSetLayoutBinding> layoutBindings =
    {
            { 0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT },
            { 1, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT }
    };

    mesh.descriptorSetLayout = std::make_shared<DescriptorSetLayout>(layoutBindings);
    mesh.descriptorSetLayout->Compile(device);

    mesh.descriptorPool = std::make_shared<DescriptorPool>(device);
    mesh.descriptorPool->SetDescriptorLayout(mesh.descriptorSetLayout);
    mesh.descriptorPool->Allocate(*this);

    for(size_t i = 0; i < swapchainImages.size(); i++) {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = mesh.uboBuffer;
        bufferInfo.offset = mesh.GetUniformBufferOffset(i);
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo = {};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = mesh.vulkanTexture->GetImageView();
        imageInfo.sampler = mesh.vulkanSampler->GetSampler();


        std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = mesh.descriptorPool->HandleToDescriptor(i);
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = mesh.descriptorPool->HandleToDescriptor(i);
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void Renderer::CreateTextureImage(Image& img, std::shared_ptr<VulkanImage>& vulkanTexture) {
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    VulkanMemory::CreateBufferAndBindMemory(device, physicalDevice, img.GetSize(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);


    VulkanMemory::CopyMemoryToGpu(device, stagingBufferMemory, img.GetData(), img.GetSize());

    vulkanTexture = std::make_shared<VulkanImage>(device, physicalDevice, img.GetWidth(), img.GetHeight(), VK_FORMAT_R8G8B8A8_SRGB,
            VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    vulkanTexture->TransitionImageLayout(device, graphicCommandPool, graphicsQueue, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    vulkanTexture->CopyBufferToImage(device, graphicCommandPool, graphicsQueue, stagingBuffer);
    vulkanTexture->TransitionImageLayout(device, graphicCommandPool, graphicsQueue, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);

    vulkanTexture->CreateImageView(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
}

void Renderer::CreateTextureSampler(std::shared_ptr<VulkanSampler>& vulkanSampler) {
    vulkanSampler = std::make_shared<VulkanSampler>(device);
}

void Renderer::CreateDepthResources() {
    VkFormat depthFormat = FindDepthBufferFormat();

    depthImage = std::make_shared<VulkanImage>(device, physicalDevice, swapchainExtent.width, swapchainExtent.height, depthFormat, 
                    VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    depthImage->CreateImageView(depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void Renderer::DestroyDepthResources() {
    depthImage->FreeImage();
}
