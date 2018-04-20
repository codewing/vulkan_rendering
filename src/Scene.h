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

public:
    Scene();

    std::vector<Vertex> getVertices();

};


#endif //VULKAN_VOXEL_SCENE_H
