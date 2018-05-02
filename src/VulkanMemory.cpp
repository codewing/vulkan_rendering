//
// Created by codewing on 23/04/2018.
//

#include <stdexcept>
#include "VulkanMemory.h"
#include "Utilities.h"

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

void VulkanMemory::CreateBufferAndBindMemory(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size,
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

    // check the memory requirements
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    // allocate memory based on requirements
    VkMemoryAllocateInfo memoryAllocateInfo{};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize = memRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = VulkanMemory::FindMemoryType(
            physicalDevice,
            memRequirements.memoryTypeBits,
            memoryPropertyFlags
    );

    ErrorCheck(vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &bufferMemory));

    // bind the memory
    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}
