//
// Created by codewing on 20.04.2018.
//

#ifndef VULKAN_VOXEL_SCENE_H
#define VULKAN_VOXEL_SCENE_H

#include <vector>
#include "vertex.h"

class Scene {

private:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

public:
    Scene();

    std::vector<Vertex> GetVertices();
    std::vector<uint32_t> GetIndices();

};


#endif //VULKAN_VOXEL_SCENE_H
