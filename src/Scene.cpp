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
#include "Image.h"
#include "MeshImporter.h"


Scene::Scene() {}

void Scene::Setup() {

    {   // Two textured quads
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
        mesh->SetTexture(std::make_shared<Image>("assets/textures/statue.jpg"));

        meshes.push_back(mesh);
    }


    {   // load house model - https://vulkan-tutorial.com/Loading_models
        std::shared_ptr<Mesh> mesh = MeshImporter::LoadObjModel("assets/models/chalet.obj", "assets/textures/chalet.jpg");

        meshes.push_back(mesh);
    }


}

void Scene::Teardown(Renderer& renderer) {
    for(auto& mesh : meshes) {
        mesh->FreeMesh(renderer);
    }
}

void Scene::UpdateCamera(float deltaSinceStart, float aspectRatio) {

    view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
    proj[1][1] *= -1; // flip the image
    for(auto& mesh : meshes) {
        // TODO: move to model activeCamera.model = glm::rotate(glm::mat4(1.0f), deltaSinceStart * glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        for(auto& ubo : mesh->GetUBO()) {
            ubo.model = glm::rotate(glm::mat4(1.0f), deltaSinceStart * glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            ubo.view = view;
            ubo.proj = proj;
        }
    }
}
