//
// Created by codewing on 20.04.2018.
//

#include "Scene.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene() {
    vertices = {
            {{-0.5f,  -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f,  -0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f},  {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f},  {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

            {{-0.5f,  -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f,  -0.5f, -0.5f},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, 0.5f, -0.5f},  {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            {{-0.5f, 0.5f, -0.5f},  {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };

    indices = {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4
    };
}

std::vector<Vertex> Scene::GetVertices() {
    return vertices;
}

std::vector<uint32_t> Scene::GetIndices() {
    return indices;
}

void Scene::UpdateCamera(float deltaSinceStart, float aspectRatio) {
    activeCamera.model = glm::rotate(glm::mat4(1.0f), deltaSinceStart * glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    activeCamera.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    activeCamera.proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
    activeCamera.proj[1][1] *= -1; // flip the image
}

UniformBufferObject& Scene::GetActiveCamera() {
    return activeCamera;
}
