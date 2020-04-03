//
// Created by codewing on 12/03/2020.
//

#include "MeshImporter.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <iostream>
#include <unordered_map>

#include "Vertex.h"
#include "Mesh.h"
#include "Image.h"


std::shared_ptr<Mesh> MeshImporter::LoadObjModel(const std::string& objPath, const std::string& texturePath) {
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
    
    LoadObjMesh(objPath, *mesh);
    LoadTexture(texturePath, *mesh);

    return mesh;
}

void MeshImporter::LoadObjMesh(const std::string& objPath, Mesh& mesh) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string error, warning;

    if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, objPath.c_str())) {
        throw std::runtime_error(error);
    }

    if(!warning.empty()) {
        std::cout << warning << std::endl;
    }

    std::vector<Vertex> vertices;
    std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
    std::vector<uint32_t> indices;

    for(const auto& shape : shapes) {
        for(const auto& index : shape.mesh.indices) {
            Vertex vertex = {};
            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };
            
            vertex.color = { 1.0f, 1.0f, 1.0f};

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            vertices.emplace_back(vertex);
            indices.push_back(uniqueVertices[vertex]);
        }
    }

    mesh.SetVertices(vertices);
    mesh.SetIndices(indices);
}

void MeshImporter::LoadTexture(const std::string& texturePath, Mesh& mesh) {
    std::shared_ptr<Image> texture = std::make_shared<Image>(texturePath.c_str());

    mesh.SetTexture(texture);
}