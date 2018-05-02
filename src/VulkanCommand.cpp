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

VkCommandBuffer VulkanCommand::CreateCopyBufferCommand(VkDevice device, VkCommandPool commandPool, VkBuffer srcBuffer,
                                            VkBuffer dstBuffer, VkDeviceSize size) {
    // Create command buffer for copying
    VkCommandBufferAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    // Start command buffer
    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    // Create the copy command
    VkBufferCopy copyRegion {};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    // End command Buffer
    vkEndCommandBuffer(commandBuffer);

    return commandBuffer;
}

void VulkanCommand::SyncExecuteSingleCommand(VkDevice device, VkCommandPool pool, VkQueue queue,
                                             VkCommandBuffer commandBuffer) {
    VkSubmitInfo submitInfo {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(queue); // use fence instead of waitIdle with multiple transfers

    // cleanup
    vkFreeCommandBuffers(device, pool, 1, &commandBuffer);
}
