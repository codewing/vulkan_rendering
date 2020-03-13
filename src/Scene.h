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
    std::shared_ptr<Renderer> renderer;

    glm::mat4 view;
    glm::mat4 proj;

public:
    Scene(std::shared_ptr<Renderer> renderer);

    void Setup();
    void Teardown();

    void UpdateCamera(float deltaTime, float aspectRatio);
};
