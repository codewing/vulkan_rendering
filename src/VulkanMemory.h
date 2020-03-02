//
// Created by codewing on 23/04/2018.
//

#ifndef VULKAN_VOXEL_VULKANMEMORY_H
#define VULKAN_VOXEL_VULKANMEMORY_H


#include <vulkan/vulkan.h>

class VulkanMemory {

private:
    static void AllocateMemory(const VkDevice &device, const VkPhysicalDevice &physicalDevice,
                               VkDeviceMemory* bufferMemory, VkMemoryPropertyFlags memoryPropertyFlags, const VkMemoryRequirements& memRequirements);

public:
    static void CreateBufferAndBindMemory(const VkDevice& device, const VkPhysicalDevice& physicalDevice, VkDeviceSize size,
                                          VkBufferUsageFlags usageFlags,
                                          VkMemoryPropertyFlags memoryPropertyFlags, VkBuffer &buffer,
                                          VkDeviceMemory &bufferMemory);

    static void AllocateMemoryBuffer(const VkDevice &device, const VkPhysicalDevice &physicalDevice,
                               VkBuffer& buffer, VkDeviceMemory* bufferMemory, VkMemoryPropertyFlags memoryPropertyFlags);

    static void AllocateMemoryImage(const VkDevice &device, const VkPhysicalDevice &physicalDevice,
                                     VkImage& image, VkDeviceMemory* imageMemory, VkMemoryPropertyFlags memoryPropertyFlags);

    static uint32_t
    FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

};


#endif //VULKAN_VOXEL_VULKANMEMORY_H