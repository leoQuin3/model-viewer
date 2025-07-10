#pragma once

#include <string>
#include <vector>
#include <assimp/scene.h>
#include "shader.h"
#include "mesh.h"

class Model
{
public:
    Model(const std::string FILE_PATH);

    void draw();
    void addMesh(Mesh &mesh);

private:
    std::vector<Mesh> meshes;
    std::string directory;

    void processNode(aiNode* node, const aiScene* scene);
    void extractMesh(aiMesh* nodeMesh, const aiScene* scene);
};