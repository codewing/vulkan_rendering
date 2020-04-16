//
// Created by codewing on 23/04/2018.
//

#pragma once


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

    static void CopyMemoryToGpu(VkDevice device, VkDeviceMemory deviceMemory, void* data, VkDeviceSize size, VkDeviceSize offset = 0);

};
