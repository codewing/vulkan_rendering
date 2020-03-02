//
// Created by codewing on 28/02/2020.
//

#pragma once

#include <vulkan/vulkan.h>

class VulkanImage {

private:
    VkImage textureImage = VK_NULL_HANDLE;
    VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;

    uint32_t width, height;
    const VkDevice& device;

public:

    VulkanImage(const VkDevice& device, const VkPhysicalDevice& physicalDevice, uint32_t width, uint32_t height,
                VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);

    void TransitionImageLayout(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void CopyBufferToImage(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkBuffer buffer);

    void FreeImage();
};

