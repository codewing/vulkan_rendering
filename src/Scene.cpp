//
// Created by codewing on 20.04.2018.
//

#include "Scene.h"

Scene::Scene() {
    vertices = {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };
}

std::vector<Vertex> Scene::getVertices() {
    return vertices;
}
