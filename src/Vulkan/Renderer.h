//
// Created by codewing on 20/10/2017.
//

#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include "SwapChainSupportDetails.h"
#include "../Scene.h"
#include "../Vertex.h"
#include "../UniformBufferObject.h"
#include "Pipeline/Pipeline.h"

class Window;
class QueueFamilyIndices;
class VulkanImage;
class VulkanSampler;
class Image;
class DescriptorPool;

class Renderer {

public:

    Renderer(std::shared_ptr<Window> window);
    ~Renderer();

    void SetupRenderer(std::shared_ptr<Scene> scene);

    bool Run();

    void RecreateSwapchain();

private:
    // Variables
    std::shared_ptr<Window> window;
    std::shared_ptr<Scene> currentScene;

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
    std::shared_ptr<VulkanImage> depthImage = nullptr;

    VkRenderPass renderPass;
    std::shared_ptr<Pipeline> graphicsPipeline;
    std::shared_ptr<PipelineLayout> pipelineLayout;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    VkCommandPool graphicCommandPool;
    VkCommandPool transferCommandPool;
    std::vector<VkCommandBuffer> graphicCommandBuffers;

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

    void CreateGraphicsPipeline();
    void DestroyGraphicsPipeline();

    void CreateFramebuffers();
    void DestroyFramebuffers();

    void CreateMeshBuffer(VkDeviceSize size, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void DestroyBuffer(VkBuffer buffer, VkDeviceMemory bufferMemory);

    void CopyDataToBuffer(void* data, VkDeviceSize dataSize, VkBuffer buffer, VkDeviceSize bufferOffset);

    void CreateVertexBuffer(std::vector<Vertex>& vertices);
    void DestroyVertexBuffer();

    void CreateIndexBuffer(std::vector<uint32_t>& indices);
    void DestroyIndexBuffer();

    void CreateUniformBuffers();
    void UpdateUniformBuffer(VkDeviceMemory uniformBufferMemory, UniformBufferObject& ubo, VkDeviceSize bufferOffset);
    void DestroyUniformBuffers();

    void CreateCommandPools();
    void DestroyCommandPools();

    void CreateCommandBuffers();

    void CreateDescriptors(std::shared_ptr<DescriptorPool> descriptorPool, VkImageView imageView, VkSampler sampler);

    void DrawFrame();

    void CreateSemaphores();
    void DestroySemaphores();

    bool CheckAllValidationLayersSupported();

    void CreateDepthResources();
    void DestroyDepthResources();

    void CreateTextureImage(Image& img, std::shared_ptr<VulkanImage> vulkanTexture);
    void CreateTextureSampler(std::shared_ptr<VulkanSampler> vulkanSampler);

    std::vector<const char *> GetRequiredExtensions();
    bool IsDeviceSuitable(VkPhysicalDevice vkPhysicalDevice);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice vkPhysicalDevice);
    SwapChainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device);
    VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
    VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR &capabilities);
    VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat FindDepthBufferFormat();
    bool HasStencilComponent(VkFormat format);

    void SetupDebugCallbacks();
    static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location,
                  int32_t code, const char *layerPrefix, const char *msg, void *userData);
    VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback);
    void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback,
                                       const VkAllocationCallbacks *pAllocator);


    friend class Mesh; friend class DescriptorSetLayout; friend class DescriptorPool;
};
