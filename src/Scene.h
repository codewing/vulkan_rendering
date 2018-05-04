//
// Created by codewing on 20.04.2018.
//

#ifndef VULKAN_VOXEL_SCENE_H
#define VULKAN_VOXEL_SCENE_H

#include <vector>
#include "vertex.h"
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


#endif //VULKAN_VOXEL_SCENE_H
