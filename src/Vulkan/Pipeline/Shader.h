//
// Created by codewing on 13/03/2020.
//

#pragma once

#include <string>

#include <vulkan/vulkan.h>
#include <memory>

class ShaderModule {

public:
    std::string shaderPath;

    explicit ShaderModule(const std::string& shaderPath);
    ~ShaderModule();

    VkShaderModule Compile(VkDevice device);
    VkShaderModule Handle();

private:
    // Compiled Variables
    VkShaderModule shaderModule = VK_NULL_HANDLE;
    VkDevice device;
};

class ShaderStage {

public:
    VkShaderStageFlagBits stageFlags;
    std::shared_ptr<ShaderModule> shaderModule;
    std::string entry_point;

    ShaderStage(VkShaderStageFlagBits stageFlags, const std::shared_ptr<ShaderModule> &shaderModule,
                const std::string &entryPoint = "main");

};

