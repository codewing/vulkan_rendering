//
// Created by codewing on 23/04/2018.
//

#ifndef VULKAN_VOXEL_VULKANMEMORY_H
#define VULKAN_VOXEL_VULKANMEMORY_H


#include <vulkan/vulkan.h>

class VulkanMemory {
    static uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
};


#endif //VULKAN_VOXEL_VULKANMEMORY_H
