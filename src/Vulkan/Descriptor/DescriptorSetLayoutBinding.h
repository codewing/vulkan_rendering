//
// Created by codewing on 13/03/2020.
//


#pragma once


#include <cstdint>
#include <vulkan/vulkan.h>

class DescriptorSetLayoutBinding {

public:
    DescriptorSetLayoutBinding(uint32_t bindingIndex, uint32_t bindingCount, VkDescriptorType descriptorType,
                               VkShaderStageFlags stageFlags);

    uint32_t binding = 0;
    uint32_t descriptorCount = 1;
    VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    VkShaderStageFlags  stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
};


