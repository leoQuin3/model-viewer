#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(const std::string FILE_PATH)
{
    // TODO: Load model
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
