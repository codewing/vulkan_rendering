//
// Created by codewing on 20/10/2017.
//

#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include "SwapChainSupportDetails.h"
#include "Scene.h"

class Window;
class QueueFamilyIndices;
class VulkanImage;
class VulkanSampler;

class Renderer {

public:

    Renderer(std::shared_ptr<Scene> scene, int width, int height);
    ~Renderer();

    bool Run();

    void RecreateSwapchain();

private:
    // Variables
    std::shared_ptr<Window> window;
    std::shared_ptr<Scene> scene;

    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;

    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;
    VkQueue transferQueue = VK_NULL_HANDLE;

    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;
    std::vector<VkFramebuffer> swapchainFramebuffers;

    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    std::shared_ptr<VulkanImage> texture = nullptr;
    std::shared_ptr<VulkanSampler> sampler = nullptr;

    VkCommandPool graphicCommandPool;
    VkCommandPool transferCommandPool;
    std::vector<VkCommandBuffer> graphicCommandBuffers;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;

    VkDebugReportCallbackEXT callback = VK_NULL_HANDLE;

    const std::vector<const char *> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};
    const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

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
    void CleanupSwapchain();

    void GetSwapchainImages();

    void CreateImageViews();
    void DestroyImageViews();

    void CreateRenderPass();
    void DestroyRenderPass();

    void CreateDescriptorSetLayout();
    void DestroyDescriptorSetLayout();

    void CreateGraphicsPipeline();
    void DestroyGraphicsPipeline();

    void CreateFramebuffers();
    void DestroyFramebuffers();

    void CreateVertexBuffer();
    void DestroyVertexBuffer();

    void CreateIndexBuffer();
    void DestroyIndexBuffer();

    void CreateUniformBuffers();
    void UpdateUniformBuffers(int currentImage);
    void DestroyUniformBuffers();

    void CreateCommandPools();
    void DestroyCommandPools();

    void CreateCommandBuffers();

    void CreateDescriptorPool();
    void DestroyDescriptorPool();
    void CreateDescriptorSets();

    void DrawFrame();

    void CreateSemaphores();
    void DestroySemaphores();

    bool CheckAllValidationLayersSupported();

    void CreateTextureImage();
    void CreateTextureSampler();

    std::vector<const char *> GetRequiredExtensions();
    bool IsDeviceSuitable(VkPhysicalDevice vkPhysicalDevice);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice vkPhysicalDevice);
    SwapChainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
    VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    void SetupDebugCallbacks();
    static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location,
                  int32_t code, const char *layerPrefix, const char *msg, void *userData);
    VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback);
    void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback,
                                       const VkAllocationCallbacks *pAllocator);

    VkShaderModule CreateShaderModule(const std::vector<char> &code);

};
