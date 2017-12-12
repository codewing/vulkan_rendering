//
// Created by codewing on 20/10/2017.
//

#ifndef VULKAN_VOXEL_RENDERER_H
#define VULKAN_VOXEL_RENDERER_H

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include "SwapChainSupportDetails.h"

class Window;
class QueueFamilyIndices;

class Renderer {

private:
    // Variables
    std::shared_ptr<Window> window;

    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;

    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;

    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;
    std::vector<VkFramebuffer> swapchainFramebuffers;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    VkDebugReportCallbackEXT callback = VK_NULL_HANDLE;

    const std::vector<const char*> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};
    const std::vector<const char*> deviceExtensions  = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    // Methods
    void InitVulkan();
    void DeInitVulkan();

    void CreateInstance();
    void DestroyInstance();

    void CreateSurface();
    void DestroySurface();

    void SetupPhysicalDevice();

    void InitLogicalDevice();
    void DeInitLogicalDevice();

    void CreateSwapchain();
    void DestroySwapchain();
    void GetSwapchainImages();
    void CreateImageViews();
    void DestroyImageViews();

    void CreateRenderPass();
    void DestroyRenderPass();
    void CreateGraphicsPipeline();
    void DestroyGraphicsPipeline();

    void CreateFramebuffers();
    void DestroyFramebuffers();

    void CreateCommandPool();
    void DestroyCommandPool();
    void CreateCommandBuffers();

    bool CheckAllValidationLayersSupported();
    std::vector<const char*> GetRequiredExtensions();

    // Debug callbacks
    bool IsDeviceSuitable(VkPhysicalDevice vkPhysicalDevice);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice vkPhysicalDevice);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice vkPhysicalDevice);

    SwapChainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
    VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    void SetupDebugCallbacks();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);
    VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
    void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
    VkShaderModule CreateShaderModule(const std::vector<char>& code);

public:

    Renderer(int width, int height);
    ~Renderer();

    bool Run();

};


#endif //VULKAN_VOXEL_RENDERER_H
