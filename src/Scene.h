//
// Created by codewing on 20.04.2018.
//

#pragma once

#include <vector>
#include "Vertex.h"
#include "UniformBufferObject.h"

class Scene {

private:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    UniformBufferObject activeCamera;

public:
    Scene();

    void UpdateCamera(float DeltaTime);

    std::vector<Vertex> GetVertices();
    std::vector<uint32_t> GetIndices();
    UniformBufferObject& GetActiveCamera();

};
