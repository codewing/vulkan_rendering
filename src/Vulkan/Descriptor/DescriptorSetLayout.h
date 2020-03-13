//
// Created by codewing on 13/03/2020.
//

#pragma once

#include <vector>

#include "DescriptorSetLayoutBinding.h"


class DescriptorSetLayout {

public:
    DescriptorSetLayout(const std::vector<DescriptorSetLayoutBinding> &bindings);
    ~DescriptorSetLayout();

    void Compile(VkDevice device);
    VkDescriptorSetLayout Handle() const;

    std::vector<DescriptorSetLayoutBinding> bindings;

private:
    VkDescriptorSetLayout descriptorSetLayout;

    VkDevice device;
};


