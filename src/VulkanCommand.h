//
// Created by codewing on 24/04/2018.
//

#ifndef VULKAN_VOXEL_VULKANCOMMAND_H
#define VULKAN_VOXEL_VULKANCOMMAND_H


#include "VulkanDevice.h"

class VulkanCommand {

public:
    static void CreateCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
                                  QueueFamilyType queueFamilyType, VkCommandPool *commandPool);

    static void SyncExecuteSingleCommand(VkDevice device, VkCommandPool pool, VkQueue queue,
                                         VkCommandBuffer commandBuffer);

    static VkCommandBuffer CreateCopyBufferCommand(VkDevice device, VkCommandPool commandPool, VkBuffer srcBuffer,
                                                   VkBuffer dstBuffer, VkDeviceSize size);

};


#endif //VULKAN_VOXEL_VULKANCOMMAND_H
