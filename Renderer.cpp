//
// Created by codewing on 20/10/2017.
//
#include "BUILD_OPTIONS.h"

#include "Renderer.h"
#include "Utilities.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>
#include <set>
#include "Window.h"
#include "QueueFamilyIndices.h"

Renderer::Renderer(int width, int height) {
    window = new Window(width, height);
    InitVulkan();
}

Renderer::~Renderer() {
    DeInitVulkan();
}

void Renderer::InitVulkan() {
    CreateInstance();
    SetupDebugCallbacks();
    CreateSurface();
    SetupPhysicalDevice();
    InitLogicalDevice();
}

void Renderer::DeInitVulkan() {
    DeInitLogicalDevice();
    DestroySurface();
    DestroyDebugReportCallbackEXT(instance, callback, nullptr);
    DestroyInstance();
}

void Renderer::CreateInstance() {

#ifdef BUILD_ENABLE_VULKAN_DEBUG
    if(!CheckAllValidationLayersSupported()){
        throw std::runtime_error("more validation layers requested than available");
    }
#endif

    VkApplicationInfo applicationInfo {};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = "Voxel World Generation";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = "No Engine";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.apiVersion = VK_API_VERSION_1_0;

    // Get extensions via GLFW
    unsigned int glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    VkInstanceCreateInfo instanceCreateInfo {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledExtensionCount = glfwExtensionCount;
    instanceCreateInfo.ppEnabledExtensionNames = glfwExtensions;

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
    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayerProperties) {
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
    std::vector<const char*> extensions;

    unsigned int glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

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
    VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfoEXT {};
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
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if (func != nullptr) {
        func(instance, callback, pAllocator);
    }
}

bool Renderer::Run() {
    if(window != nullptr) {
        return window->Update();
    }
    return true;
}

void Renderer::SetupPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (IsDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

bool Renderer::IsDeviceSuitable(VkPhysicalDevice vkPhysicalDevice) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    QueueFamilyIndices indices = FindQueueFamilies(vkPhysicalDevice);

    vkGetPhysicalDeviceProperties(vkPhysicalDevice, &deviceProperties);
    vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &deviceFeatures);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
           && deviceFeatures.geometryShader
           && indices.isComplete();
}

QueueFamilyIndices Renderer::FindQueueFamilies(VkPhysicalDevice vkPhysicalDevice) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, queueFamilies.data());

    int queueIndex = 0;
    for(const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.SetGraphicsFamily(queueIndex);
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, queueIndex, surface, &presentSupport);

        if (queueFamily.queueCount > 0 && presentSupport) {
            indices.SetPresentFamily(queueIndex);
        }

        if (indices.isComplete()) {
            break;
        }

        queueIndex++;
    }

    return indices;
}

void Renderer::InitLogicalDevice() {
    QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfoVector;
    std::set<int> uniqueQueueFamilies = {indices.GetGraphicsFamily(), indices.GetPresentFamily()};

    float queuePriority = 1.0f;

    for (int queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfoVector.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures {};

    VkDeviceCreateInfo deviceCreateInfo {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfoVector.data();
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfoVector.size());
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    deviceCreateInfo.enabledExtensionCount = 0;

#if BUILD_ENABLE_VULKAN_DEBUG
    deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
#else
    deviceCreateInfo.enabledLayerCount = 0;
#endif

    ErrorCheck(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device));

    vkGetDeviceQueue(device, indices.GetGraphicsFamily(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.GetPresentFamily(), 0, &presentQueue);
}

void Renderer::DeInitLogicalDevice() {
    vkDestroyDevice(device, nullptr);
}

void Renderer::CreateSurface() {
    if (glfwCreateWindowSurface(instance, window->GetGLFWwindow(), nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

void Renderer::DestroySurface() {
    vkDestroySurfaceKHR(instance, surface, nullptr);
}


