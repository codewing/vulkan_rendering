//
// Created by codewing on 17/03/2020.
//

#pragma once

#include <memory>
#include <vulkan/vulkan.h>

#include "DescriptorSetLayout.h"

class Renderer;

class DescriptorPool {

private:
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;

    std::shared_ptr<DescriptorSetLayout> descriptorLayout;

    std::vector<VkDescriptorSet> descriptorSets;

    const VkDevice& device;

public:
    DescriptorPool(const VkDevice& device);
    void FreeDescriptorPool();

    void SetDescriptorLayout(std::shared_ptr<DescriptorSetLayout> layout);
    void Allocate(Renderer& renderer);
    void Deallocate();

    VkDescriptorPool Handle() { return descriptorPool; };
    VkDescriptorSet HandleToDescriptor(uint32_t index) { return descriptorSets[index]; };
};