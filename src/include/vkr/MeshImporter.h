//
// Created by codewing on 12/03/2020.
//

#pragma once

#include <string>
#include <memory>

class Mesh;

class MeshImporter {

private:

    static void LoadObjMesh(const std::string& objPath, Mesh& mesh);
    static void LoadTexture(const std::string& texturePath, Mesh& mesh);

public:

    static std::shared_ptr<Mesh> LoadObjModel(const std::string& objPath, const std::string& texturePath);

};
