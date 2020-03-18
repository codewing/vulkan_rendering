//
// Created by codewing on 17/03/2020.
//

#include "DescriptorPool.h"

#include "../Renderer.h"
#include "../VulkanUtilities.h"


DescriptorPool::DescriptorPool(const VkDevice& device) : device(device) {

}

void DescriptorPool::FreeDescriptorPool() {
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
}

void DescriptorPool::SetDescriptorLayout(std::shared_ptr<DescriptorSetLayout> layout) {
    this->descriptorLayout = layout;
}

void DescriptorPool::Allocate(Renderer& renderer) {
    std::vector<VkDescriptorPoolSize> poolSizes = descriptorLayout->GetDescriptorPoolSize();

    VkDescriptorPoolCreateInfo poolCreateInfo {};
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolCreateInfo.pPoolSizes = poolSizes.data();
    poolCreateInfo.maxSets = static_cast<uint32_t>(renderer.swapchainImages.size());

    ErrorCheck(vkCreateDescriptorPool(device, &poolCreateInfo, nullptr, &descriptorPool));

    VkDescriptorSetLayout layoutHandle = descriptorLayout->Handle();
    std::vector<VkDescriptorSetLayout> layouts;
    for(int i = 0; i < renderer.swapchainImages.size(); i++) {
        layouts.push_back(layoutHandle);
    }

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(layouts.size());
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(renderer.swapchainImages.size());

    ErrorCheck(vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()));
}

void DescriptorPool::Deallocate() {
    vkFreeDescriptorSets(device, descriptorPool, descriptorSets.size(), descriptorSets.data());
}