//
// Created by codewing on 24/04/2018.
//

#include "VulkanCommand.h"
#include "Utilities.h"

void VulkanCommand::CreateCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, QueueFamilyType queueFamilyType, VkCommandPool* commandPool){
    QueueFamilyIndices indices = VulkanDevice::FindQueueFamilies(physicalDevice, surface, queueFamilyType);
    uint32_t queueFamilyIndex;

    switch (queueFamilyType) {
        case QueueFamilyType::GRAPHICS_WITH_PRESENT_FAMILY:{
            queueFamilyIndex = indices.GetGraphicsFamily();
        }break;

        case QueueFamilyType::TRANSFER_FAMILY:{
            queueFamilyIndex = indices.GetTransferFamily();
        }break;
    }

    VkCommandPoolCreateInfo poolInfo {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndex;
    poolInfo.flags = 0; // Optional

    ErrorCheck(vkCreateCommandPool(device, &poolInfo, nullptr, commandPool));
}