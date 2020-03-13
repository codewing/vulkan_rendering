//
// Created by codewing on 20.04.2018.
//

#include "Scene.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "Vertex.h"

Scene::Scene(std::shared_ptr<Renderer> renderer) : renderer(renderer) {}

void Scene::Setup() {
    std::vector<Vertex> vertices = {
            {{-0.5f,  -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f,  -0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f},  {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f},  {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

            {{-0.5f,  -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f,  -0.5f, -0.5f},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, 0.5f, -0.5f},  {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            {{-0.5f, 0.5f, -0.5f},  {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };

    std::vector<uint32_t> indices = {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4
    };

    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
    mesh->SetVertices(vertices);
    mesh->SetIndices(indices);
    mesh->CreateBuffer();
    mesh->CopyDataToGPU();
    // bind ubo buffer
    // record mesh commands ((graphic)commandBuffer - bind vertex index buffer descSet)

    meshes.push_back(mesh);

    // descriptor sets
    // build command buffers
}

void Scene::Teardown() {
    for(auto& mesh : meshes) {
        mesh->DestroyBuffer();
    }
}

void Scene::UpdateCamera(float deltaSinceStart, float aspectRatio) {
    // TODO: move to model activeCamera.model = glm::rotate(glm::mat4(1.0f), deltaSinceStart * glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
    proj[1][1] *= -1; // flip the image
}
