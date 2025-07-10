#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

Model::Model(const std::string FILE_PATH)
{
    // TODO: Load model
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(FILE_PATH, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
        return;
    }

    // Store asset directory
    directory = FILE_PATH;

    // Read through each node recursively
    processNode(scene->mRootNode, scene);
}

void Model::draw()
{
    for (Mesh &mesh : this->meshes)
        mesh.draw();
}

void Model::addMesh(Mesh &mesh)
{
    this->meshes.push_back(mesh);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // Get all meshes
    for (int i = 0; i < node->mNumMeshes; i++)
    {
        unsigned int meshIndex = node->mMeshes[i];
        aiMesh* sceneMesh = scene->mMeshes[meshIndex];

        // Obtain meshes
        extractMesh(sceneMesh, scene);
    }

    // Read through each children
    for (int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

// TODO: Construct mesh and push into meshes vector
void Model::extractMesh(aiMesh *nodeMesh, const aiScene *scene)
{
    // DEBUG: print each mesh name
    std::cout << nodeMesh->mName.C_Str() << "\n";
}
