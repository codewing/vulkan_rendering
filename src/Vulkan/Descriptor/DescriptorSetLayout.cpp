//
// Created by codewing on 13/03/2020.
//

#include "DescriptorSetLayout.h"
#include "../VulkanUtilities.h"


DescriptorSetLayout::DescriptorSetLayout(const std::vector<DescriptorSetLayoutBinding> &bindings) : bindings(
        bindings) {}

void DescriptorSetLayout::Compile(VkDevice device) {
    this->device = device;

    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;

    for(const auto& binding : bindings) {
        VkDescriptorSetLayoutBinding setLayoutBinding = {};
        setLayoutBinding.descriptorType  = binding.descriptorType;
        setLayoutBinding.stageFlags      = binding.stageFlags;
        setLayoutBinding.binding         = binding.binding;
        setLayoutBinding.descriptorCount = binding.descriptorCount;
        setLayoutBindings.emplace_back(setLayoutBinding);
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
    layoutInfo.pBindings = setLayoutBindings.data();

    ErrorCheck(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout));
}

VkDescriptorSetLayout DescriptorSetLayout::Handle() const {
    return descriptorSetLayout;
}

DescriptorSetLayout::~DescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
}
