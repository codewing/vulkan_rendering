//
// Created by codewing on 24/04/2018.
//

#include <vector>
#include <stdexcept>
#include "VulkanDevice.h"

QueueFamilyIndices VulkanDevice::FindQueueFamilies(VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR surface, QueueFamilyType type) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, queueFamilies.data());

    int queueIndex = 0;
    for(const auto& queueFamily : queueFamilies) {
        if(queueFamily.queueCount <= 0){
            continue;
        }

        if(QueueFamilyType::GRAPHICS_WITH_PRESENT_FAMILY == type){
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.SetGraphicsFamily(queueIndex);
            }

            VkBool32 presentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, queueIndex, surface, &presentSupport);

            if (presentSupport) {
                indices.SetPresentFamily(queueIndex);
            }

            if (indices.isGraphicsWithPresentFamilySet()) {
                break;
            }
        } else if(QueueFamilyType::TRANSFER_FAMILY == type) {
            if ((queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) && !(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
                indices.SetTransferFamily(queueIndex);
                break;
            }
        } else {
            throw std::runtime_error("Missing case for QueueFamilyType");
        }


        queueIndex++;
    }

    return indices;
}