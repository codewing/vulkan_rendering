//
// Created by codewing on 12/03/2020.
//

#include "vkr/Mesh.h"

#include "vkr/Vertex.h"
#include "vkr/Image.h"
#include "vkr/Vulkan/Renderer.h"
#include "Vulkan/Image/VulkanImage.h"
#include "Vulkan/Image/VulkanSampler.h"
#include "Vulkan/Descriptor/DescriptorPool.h"


Mesh::Mesh() {

}

Mesh::~Mesh() {

}

void Mesh::SetVertices(std::vector<Vertex> newVertices) {
    this->vertices = std::move(newVertices);
}

void Mesh::SetIndices(std::vector<uint32_t> newIndices) {
    this->indices = std::move(newIndices);
}

void Mesh::SetTexture(std::shared_ptr<Image> newImage) {
    this->texture = std::move(newImage);
}

void Mesh::CreateBuffers(Renderer& renderer) {
    vertexBufferOffset = 0;
    indexBufferOffset = sizeof(Vertex) * vertices.size();

    renderer.CreateMeshBuffer(indexBufferOffset + sizeof(uint32_t) * indices.size(), buffer, bufferMemory);

    uniformBufferOffsets.resize(renderer.swapchainImages.size());

    CreateUniformBuffers(renderer);
}

void Mesh::DestroyMeshBuffer(Renderer& renderer) {
    renderer.DestroyBuffer(buffer, bufferMemory);
}

void Mesh::DestroyUniformBuffers(Renderer &renderer) {
    renderer.DestroyBuffer(uboBuffer, uboBufferMemory);
}

void Mesh::CopyDataToGPU(Renderer& renderer) {
    renderer.CopyDataToBuffer(vertices.data(), vertices.size() * sizeof(Vertex), buffer, vertexBufferOffset);
    renderer.CopyDataToBuffer(indices.data(), indices.size() * sizeof(uint32_t), buffer, indexBufferOffset);
}

void Mesh::UpdateUniformBuffer(Renderer& renderer, uint32_t imageIndex) {
    renderer.UpdateUniformBuffer(uboBufferMemory, ubos[imageIndex], uniformBufferOffsets[imageIndex]);
}

void Mesh::CreateTexture(Renderer& renderer) {
    renderer.CreateTextureImage(*texture, vulkanTexture);
}

void Mesh::CreateSampler(Renderer& renderer) {
    renderer.CreateTextureSampler(vulkanSampler);
}

void Mesh::CreateDescriptors(Renderer& renderer) {
    renderer.CreateDescriptors(*this);
}

VkDeviceSize Mesh::GetVertexBufferOffset() { 
    return vertexBufferOffset;
}

VkDeviceSize Mesh::GetIndexBufferOffset() { 
    return indexBufferOffset;
};

VkDeviceSize Mesh::GetUniformBufferOffset(int swapchainIndex) {
    return uniformBufferOffsets[swapchainIndex];
};

void Mesh::FreeMesh(Renderer& renderer) {
    DestroyMeshBuffer(renderer);
}

VkDescriptorSet Mesh::GetDescriptorSet(uint32_t frame) { 
    return descriptorPool->HandleToDescriptor(frame); 
}

void Mesh::DestroyDescriptors(Renderer &renderer) {
    descriptorSetLayout->FreeDescriptorSetLayout();
    descriptorPool->FreeDescriptorPool();
}

void Mesh::CreateUniformBuffers(Renderer& renderer) {
    double uniformOffsetAlignment = renderer.physicalDeviceProperties.limits.minUniformBufferOffsetAlignment;
    VkDeviceSize currentOffset = 0;
    for(int i = 0; i < uniformBufferOffsets.size(); i++) {
        auto num = std::ceil(currentOffset/uniformOffsetAlignment);
        uniformBufferOffsets[i] = num * uniformOffsetAlignment;
        currentOffset = uniformBufferOffsets[i] + sizeof(UniformBufferObject);
    }
    renderer.CreateUBOBuffer(currentOffset, uboBuffer, uboBufferMemory);
    ubos.resize(renderer.swapchainImages.size());
}
