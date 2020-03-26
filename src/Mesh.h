//
// Created by codewing on 12/03/2020.
//

#pragma once

#include <string>
#include <vector>
#include <memory>

#include <vulkan/vulkan.h>

#include "UniformBufferObject.h"

class Image;
class Renderer;
class VulkanImage;
class VulkanSampler;
class DescriptorPool;
class DescriptorSetLayout;
struct Vertex;

class Mesh {

private:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<UniformBufferObject> ubos;

    std::shared_ptr<Image> texture;

    // internal values
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;

    VkBuffer uboBuffer;
    VkDeviceMemory uboBufferMemory;

    VkDeviceSize vertexBufferOffset = 0;
    VkDeviceSize indexBufferOffset = 0;
    std::vector<VkDeviceSize> uniformBufferOffsets;

    std::shared_ptr<VulkanImage> vulkanTexture = nullptr;
    std::shared_ptr<VulkanSampler> vulkanSampler = nullptr;

    std::shared_ptr<DescriptorPool> descriptorPool = nullptr;
    std::shared_ptr<DescriptorSetLayout> descriptorSetLayout = nullptr;

public:
    explicit Mesh();
    virtual ~Mesh();

    void SetVertices(std::vector<Vertex> vertices);
    void SetIndices(std::vector<uint32_t> indices);
    void SetTexture(std::shared_ptr<Image> image);

    std::vector<Vertex>& GetVertices() { return vertices; };
    std::vector<uint32_t>& GetIndices() { return indices; };
    std::vector<UniformBufferObject>& GetUBO() { return ubos; };

    void CreateBuffers(Renderer& renderer);
    void DestroyBuffer(Renderer& renderer);
    void DestroyUniformBuffers(Renderer& renderer);
    void CopyDataToGPU(Renderer& renderer);
    void UpdateUniformBuffer(Renderer& renderer, uint32_t imageIndex);

    void CreateTexture(Renderer& renderer);
    void CreateSampler(Renderer& renderer);

    void CreateDescriptors(Renderer& renderer);
    void DestroyDescriptors(Renderer& renderer);

    VkDeviceSize GetVertexBufferOffset();
    VkDeviceSize GetIndexBufferOffset();
    VkDeviceSize GetUniformBufferOffset(int swapchainIndex);

    VkBuffer GetBufferHandle() { return buffer; };
    VkDescriptorSet GetDescriptorSet(uint32_t frame);
    std::shared_ptr<DescriptorSetLayout> GetDescriptorSetLayout() { return descriptorSetLayout; };

    void FreeMesh(Renderer& renderer);

    friend class Renderer;
};
