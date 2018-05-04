//
// Created by codewing on 04.05.2018.
//

#ifndef VULKAN_VOXEL_UNIFORMBUFFEROBJECT_H
#define VULKAN_VOXEL_UNIFORMBUFFEROBJECT_H

#include <glm/mat4x4.hpp>

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

#endif //VULKAN_VOXEL_UNIFORMBUFFEROBJECT_H
