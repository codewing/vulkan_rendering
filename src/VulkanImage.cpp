//
// Created by codewing on 28/02/2020.
//

#include "VulkanImage.h"
#include "VulkanMemory.h"
#include "VulkanCommand.h"

#include <stdexcept>

VulkanImage::VulkanImage(const VkDevice& device, const VkPhysicalDevice& physicalDevice, uint32_t width, uint32_t height,
        VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
        : width(width), height(height), device(device) {

    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0; // Optional - e.g. sparse textures/volumes

    if (vkCreateImage(device, &imageInfo, nullptr, &textureImage) != VK_SUCCESS) {
        throw std::runtime_error("failed to create VulkanImage!");
    }

    VulkanMemory::AllocateMemoryImage(device, physicalDevice, textureImage, &textureImageMemory, properties);

}

void VulkanImage::TransitionImageLayout(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
    VulkanCommand::TransitionImageLayout(device, commandPool, queue, textureImage, format, oldLayout, newLayout);
}

void VulkanImage::CopyBufferToImage(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkBuffer buffer) {
    VulkanCommand::CopyBufferToImage(device, commandPool, queue, buffer, textureImage, width, height);
}

void VulkanImage::FreeImage() {
    vkDestroyImage(device, textureImage, nullptr);
    vkFreeMemory(device, textureImageMemory, nullptr);
}
