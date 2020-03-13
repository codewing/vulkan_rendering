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
struct Vertex;

class Mesh {

private:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<UniformBufferObject> ubos;

    std::shared_ptr<Image> texture;

    VkBuffer buffer;
    VkDeviceMemory bufferMemory;

    std::shared_ptr<Renderer> renderer;

public:
    explicit Mesh();
    virtual ~Mesh();


    void SetVertices(std::vector<Vertex> vertices);
    void SetIndices(std::vector<uint32_t> indices);
    void SetTexture(std::shared_ptr<Image> image);

    std::vector<Vertex>& GetVertices() { return vertices; };
    std::vector<uint32_t>& GetIndices() { return indices; };
    std::vector<UniformBufferObject>& GetUBO() { return ubos; };

    void CreateBuffer();
    void DestroyBuffer();

    void CopyDataToGPU();

    VkDeviceSize GetVertexBufferOffset();
    VkDeviceSize GetIndexBufferOffset();
    VkDeviceSize GetUniformBufferOffset(int swapchainIndex);
};
