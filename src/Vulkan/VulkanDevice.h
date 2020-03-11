//
// Created by codewing on 24/04/2018.
//

#pragma once


#include <vulkan/vulkan.h>
#include "QueueFamilyIndices.h"

enum QueueFamilyType {
    GRAPHICS_WITH_PRESENT_FAMILY,
    TRANSFER_FAMILY
};

class VulkanDevice {

public:
    static QueueFamilyIndices
    FindQueueFamilies(VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR surface, QueueFamilyType type);

};
