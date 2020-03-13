//
// Created by codewing on 13/03/2020.
//

#pragma once

#include <string>
#include <vector>
#include <memory>

#include <vulkan/vulkan.h>

#include "Shader.h"
#include "../Descriptor/DescriptorSetLayout.h"
#include "../RenderContext.h"


class PipelineLayout {

public:
    ~PipelineLayout();

    void Compile(VkDevice device);
    VkPipelineLayout Handle();

    std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSetLayouts;

private:
    std::vector<VkDescriptorSetLayout> compiledDescriptorSetLayouts;
    VkPipelineLayout pipelineLayout;

    VkDevice device;
};


class Pipeline {

public:
    std::vector<ShaderStage> shaderStages;
    std::vector<VkViewport> viewports;
    std::vector<VkRect2D> scissors;

    explicit Pipeline(std::shared_ptr<PipelineLayout> pipelineLayout);
    ~Pipeline();

    void Compile(RenderContext& context);
    VkPipeline Handle();

private:
    std::shared_ptr<PipelineLayout> pipelineLayout;
    VkPipeline pipeline;

    VkDevice device;
};
