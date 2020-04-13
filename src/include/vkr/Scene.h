//
// Created by codewing on 20.04.2018.
//

#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

class Mesh;
class Renderer;

class Scene {

private:
    std::vector<std::shared_ptr<Mesh>> meshes;

    glm::mat4 view;
    glm::mat4 proj;

public:
    Scene();

    void Setup();
    void Teardown(Renderer& renderer);

    void UpdateCamera(float deltaTime, float aspectRatio);

    std::vector<std::shared_ptr<Mesh>>& GetMeshes() { return meshes; };
};
