//
// Created by codewing on 23/04/2018.
//

#include <stdexcept>
#include "VulkanMemory.h"
#include "VulkanUtilities.h"

uint32_t
VulkanMemory::FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {

    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void VulkanMemory::CreateBufferAndBindMemory(const VkDevice& device, const VkPhysicalDevice& physicalDevice, VkDeviceSize size,
                                             VkBufferUsageFlags usageFlags,
                                             VkMemoryPropertyFlags memoryPropertyFlags, VkBuffer &buffer,
                                             VkDeviceMemory &bufferMemory) {
    // create the buffer
    VkBufferCreateInfo bufferCreateInfo{};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = size;
    bufferCreateInfo.usage = usageFlags;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //only used by graphics queue

    ErrorCheck(vkCreateBuffer(device, &bufferCreateInfo, nullptr, &buffer));

    AllocateMemoryBuffer(device, physicalDevice, buffer, &bufferMemory, memoryPropertyFlags);
}

void VulkanMemory::AllocateMemory(const VkDevice &device, const VkPhysicalDevice &physicalDevice, VkDeviceMemory* bufferMemory, VkMemoryPropertyFlags memoryPropertyFlags, const VkMemoryRequirements& memRequirements) {
    // allocate memory based on requirements
    VkMemoryAllocateInfo memoryAllocateInfo{};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize = memRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = VulkanMemory::FindMemoryType(
            physicalDevice,
            memRequirements.memoryTypeBits,
            memoryPropertyFlags
    );

    ErrorCheck(vkAllocateMemory(device, &memoryAllocateInfo, nullptr, bufferMemory));
}

void
VulkanMemory::AllocateMemoryBuffer(const VkDevice &device, const VkPhysicalDevice &physicalDevice, VkBuffer &buffer,
                                   VkDeviceMemory *bufferMemory, VkMemoryPropertyFlags memoryPropertyFlags) {
    // check the buffer memory requirements
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    AllocateMemory(device, physicalDevice, bufferMemory, memoryPropertyFlags, memRequirements);

    // bind the buffer memory
    vkBindBufferMemory(device, buffer, *bufferMemory, 0);
}

void VulkanMemory::AllocateMemoryImage(const VkDevice &device, const VkPhysicalDevice &physicalDevice, VkImage &image,
                                       VkDeviceMemory *imageMemory, VkMemoryPropertyFlags memoryPropertyFlags) {
    // check the image memory requirements
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, &memRequirements);

    AllocateMemory(device, physicalDevice, imageMemory, memoryPropertyFlags, memRequirements);

    // bind the image memory
    vkBindImageMemory(device, image, *imageMemory, 0);
}

void VulkanMemory::CopyMemoryToGpu(VkDevice device, VkDeviceMemory deviceMemory, void* data, size_t size) {
    void* gpuMemory;
    vkMapMemory(device, deviceMemory, 0, size, 0, &gpuMemory);
    memcpy(gpuMemory, data, size);
    vkUnmapMemory(device, deviceMemory);
}
