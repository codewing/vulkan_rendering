//
// Created by codewing on 06/03/2020.
//

#pragma once

#include <vulkan/vulkan.h>

class VulkanSampler {

private:
    VkSampler sampler = VK_NULL_HANDLE;

    const VkDevice& device;

public:

    VulkanSampler(const VkDevice& device);
    void FreeSampler();
};


