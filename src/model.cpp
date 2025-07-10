#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

Model::Model(const std::string FILE_PATH)
{
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
    // Get meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // Obtain mesh
        unsigned int meshIndex = node->mMeshes[i];
        aiMesh* sceneMesh = scene->mMeshes[meshIndex];

        extractMesh(sceneMesh, scene);
    }

    // Read through each children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

// Copy mesh into model
void Model::extractMesh(aiMesh *nodeMesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Get vertices
    for (unsigned int i = 0; i < nodeMesh->mNumVertices; i++)
    {
        Vertex newVertex;
        
        // Copy positions 
        aiVector3D meshPos = nodeMesh->mVertices[i];
        glm::vec3 newPos;
        newPos.x = meshPos.x;
        newPos.y = meshPos.y;
        newPos.z = meshPos.z;
        newVertex.position = newPos;

        // Copy normals
        aiVector3D meshNorm = nodeMesh->mNormals[i];
        glm::vec3 newNorm;
        newNorm.x = meshNorm.x;
        newNorm.y = meshNorm.y;
        newNorm.z = meshNorm.z;
        newVertex.normal = newNorm;

        // Push to vertex list
        vertices.push_back(newVertex);
    }

    // Get indices
    for (unsigned int i = 0; i < nodeMesh->mNumFaces; i++)
    {
        aiFace face = nodeMesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Add mesh to model
    Mesh newMesh(vertices, indices);
    this->meshes.push_back(newMesh);
}