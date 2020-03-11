//
// Created by codewing on 24/04/2018.
//

#pragma once


#include "VulkanDevice.h"

class VulkanCommand {

public:
    static void CreateCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
                                  QueueFamilyType queueFamilyType, VkCommandPool *commandPool);

    static VkCommandBuffer BeginSingleTimeCommands(const VkDevice& device, VkCommandPool commandPool);
    static void ExecuteSingleTimeCommands(const VkDevice& device, VkCommandPool commandPool, VkCommandBuffer commandBuffer, VkQueue queue);


    static void CopyBuffer(VkDevice device, VkCommandPool commandPool, VkBuffer srcBuffer,
                           VkBuffer dstBuffer, VkDeviceSize size, VkQueue queue);
    static void TransitionImageLayout(VkDevice device, VkCommandPool commandPool, VkQueue queue,
                            VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    static void CopyBufferToImage(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkBuffer buffer,
                            VkImage image, uint32_t width, uint32_t height);
};
