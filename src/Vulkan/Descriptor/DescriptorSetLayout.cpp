//
// Created by codewing on 13/03/2020.
//

#include "DescriptorSetLayout.h"
#include "../VulkanUtilities.h"

#include "../Renderer.h"


DescriptorSetLayout::DescriptorSetLayout(const std::vector<DescriptorSetLayoutBinding>& bindings) : bindings(
        bindings) {}

VkDescriptorSetLayout DescriptorSetLayout::Compile(VkDevice device) {
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

    return descriptorSetLayout;
}

VkDescriptorSetLayout DescriptorSetLayout::Handle() const {
    return descriptorSetLayout;
}

std::vector<VkDescriptorPoolSize> DescriptorSetLayout::GetDescriptorPoolSize(uint32_t poolSize) {

    std::vector<VkDescriptorPoolSize> poolSizes;

    for(const auto& binding : bindings) {
        VkDescriptorPoolSize descriptorPoolSize;
        descriptorPoolSize.type = binding.descriptorType;
        descriptorPoolSize.descriptorCount = binding.descriptorCount * poolSize;

        poolSizes.push_back(descriptorPoolSize);
    }

    return std::move(poolSizes);
}

void DescriptorSetLayout::FreeDescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
}

DescriptorSetLayout::~DescriptorSetLayout() {}
