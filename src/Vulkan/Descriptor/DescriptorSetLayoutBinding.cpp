//
// Created by codewing on 13/03/2020.
//

#include "DescriptorSetLayoutBinding.h"

DescriptorSetLayoutBinding::DescriptorSetLayoutBinding(uint32_t bindingIndex, uint32_t bindingCount,
                                                       VkDescriptorType descriptorType, VkShaderStageFlags stageFlags)
        : binding(bindingIndex), descriptorCount(bindingCount), descriptorType(descriptorType),
          stageFlags(stageFlags) {}
