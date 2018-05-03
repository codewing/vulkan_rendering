//
// Created by codewing on 20.04.2018.
//

#include "Scene.h"

Scene::Scene() {
    vertices = {
            {{-0.5f,  -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f,  -0.5f},  {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}},
            {{0.5f, 0.5f},  {1.0f, 1.0f, 1.0f}}
    };

    indices = {
            0, 1, 2,
            2, 1, 3
    };
}

std::vector<Vertex> Scene::GetVertices() {
    return vertices;
}

std::vector<uint32_t> Scene::GetIndices() {
    return indices;
}
