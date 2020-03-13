//
// Created by codewing on 12/03/2020.
//

#include "Mesh.h"

#include "Vertex.h"
#include "Image.h"
#include "Vulkan/Renderer.h"

Mesh::Mesh() {

}

Mesh::~Mesh() {

}

void Mesh::SetVertices(std::vector<Vertex> vertices) {
    this->vertices = vertices;
}

void Mesh::SetIndices(std::vector<uint32_t> indices) {
    this->indices = indices;
}

void Mesh::SetTexture(std::shared_ptr<Image> image) {
    this->texture = image;
}

void Mesh::CreateBuffer() {
    VkDeviceSize bufferSize = sizeof(uint32_t) * indices.size()
                            + sizeof(Vertex) * vertices.size()
                            + sizeof(UniformBufferObject) * renderer->swapchainImages.size();

    renderer->CreateMeshBuffer(bufferSize, buffer, bufferMemory);
}

void Mesh::DestroyBuffer() {
    renderer->DestroyBuffer(buffer, bufferMemory);
}

void Mesh::CopyDataToGPU() {
    renderer->CopyDataToBuffer(vertices.data(), vertices.size(), buffer, GetVertexBufferOffset());
    renderer->CopyDataToBuffer(indices.data(), indices.size(), buffer, GetIndexBufferOffset());
    renderer->CopyDataToBuffer(ubos.data(), ubos.size(), buffer, GetUniformBufferOffset(0));
}

VkDeviceSize Mesh::GetVertexBufferOffset() { 
    return 0; 
}

VkDeviceSize Mesh::GetIndexBufferOffset() { 
    return sizeof(Vertex) * vertices.size(); 
};

VkDeviceSize Mesh::GetUniformBufferOffset(int swapchainIndex) { 
    return GetIndexBufferOffset() + sizeof(uint32_t) * indices.size() + sizeof(UniformBufferObject) * swapchainIndex; 
};