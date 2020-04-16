//
// Created by codewing on 04.05.2018.
//

#pragma once

#include <glm/mat4x4.hpp>

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};
