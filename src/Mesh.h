//
// Created by codewing on 12/03/2020.
//

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "UniformBufferObject.h"

class Image;
struct Vertex;

class Mesh {

private:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    UniformBufferObject ubo;

    std::shared_ptr<Image> texture;

public:
    explicit Mesh();
    virtual ~Mesh();


    void SetVertices(std::vector<Vertex> vertices);
    void SetIndices(std::vector<uint32_t> indices);
    void SetTexture(std::shared_ptr<Image> image);

    std::vector<Vertex>& GetVertices() { return vertices; };
    std::vector<uint32_t>& GetIndices() { return indices; };
    UniformBufferObject& GetUBO() { return ubo; };
};
