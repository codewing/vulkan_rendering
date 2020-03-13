//
// Created by codewing on 13/03/2020.
//

#include "Shader.h"

#include "../VulkanUtilities.h"
#include "../../Utilities.h"

ShaderModule::ShaderModule(const std::string& shaderPath) : shaderPath(shaderPath) {}

ShaderModule::~ShaderModule() {
    vkDestroyShaderModule(device, shaderModule, nullptr);
}

VkShaderModule ShaderModule::Handle() {
    return shaderModule;
}

VkShaderModule ShaderModule::Compile(VkDevice device) {
    this->device = device;

    std::vector<char> code = Utilities::ReadFile(shaderPath);

    VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.codeSize = code.size();
    shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    VkShaderModule shaderModule;
    ErrorCheck(vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule));

    return shaderModule;
}

ShaderStage::ShaderStage(VkShaderStageFlagBits stageFlags, const std::shared_ptr<ShaderModule> &shaderModule,
                         const std::string& entryPoint) : stageFlags(stageFlags), shaderModule(shaderModule),
                                                          entry_point(entryPoint) {}
