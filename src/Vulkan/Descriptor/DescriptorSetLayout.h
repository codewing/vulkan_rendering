//
// Created by codewing on 13/03/2020.
//

#pragma once

#include <vector>

#include "DescriptorSetLayoutBinding.h"


class Renderer;

class DescriptorSetLayout {

public:
    DescriptorSetLayout(const std::vector<DescriptorSetLayoutBinding> &bindings);
    ~DescriptorSetLayout();

    VkDescriptorSetLayout Compile(VkDevice device);
    VkDescriptorSetLayout Handle() const;

    std::vector<VkDescriptorPoolSize> GetDescriptorPoolSize(uint32_t poolSize);

    std::vector<DescriptorSetLayoutBinding> bindings;

private:
    VkDescriptorSetLayout descriptorSetLayout;

    VkDevice device;
};


