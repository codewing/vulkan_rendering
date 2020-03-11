//
// Created by codewing on 28/02/2020.
//

#include "VulkanImage.h"
#include "../VulkanMemory.h"
#include "../VulkanCommand.h"
#include "../VulkanUtilities.h"

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

    if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create VulkanImage!");
    }

    VulkanMemory::AllocateMemoryImage(device, physicalDevice, image, &imageMemory, properties);

}

void VulkanImage::TransitionImageLayout(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
    VulkanCommand::TransitionImageLayout(device, commandPool, queue, image, format, oldLayout, newLayout);
}

void VulkanImage::CopyBufferToImage(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkBuffer buffer) {
    VulkanCommand::CopyBufferToImage(device, commandPool, queue, buffer, image, width, height);
}

void VulkanImage::FreeImage() {
    vkDestroyImageView(device, imageView, nullptr);

    vkDestroyImage(device, image, nullptr);
    vkFreeMemory(device, imageMemory, nullptr);
}

void VulkanImage::CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags) {
    imageView = CreateImageViewForImage(device, image, format, aspectFlags);
}

VkImageView VulkanImage::CreateImageViewForImage(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.layerCount = 1;
    viewInfo.subresourceRange.levelCount = 1;

    VkImageView imageView;
    ErrorCheck(vkCreateImageView(device, &viewInfo, nullptr, &imageView));

    return imageView;
}