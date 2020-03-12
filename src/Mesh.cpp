//
// Created by codewing on 12/03/2020.
//

#include "Mesh.h"

#include "Vertex.h"
#include "Image.h"

Mesh::Mesh() {

}

Mesh::~Mesh() {

}

void Mesh::SetVertices(std::vector<Vertex> vertices) {
    this->vertices = vertices;
}

void Mesh::SetIndices(std::vector<uint32_t> indices) {
    this->indices = indices;
}

void Mesh::SetTexture(std::shared_ptr<Image> image) {
    this->texture = image;
}