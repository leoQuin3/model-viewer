#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <iostream>
#include "mesh.h"

// TODO: Implement this constructor, then refactor code
Mesh::Mesh(const std::string FILE_PATH)
{
	Assimp::Importer importer;

	// Get scene
	const aiScene* scene = importer.ReadFile(FILE_PATH, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_SplitLargeMeshes | aiProcess_GenNormals);
	if (scene == nullptr)
	{
		std::cerr << "ERROR::ASSIMP::Unable to load asset from path " << FILE_PATH << "\n";
		return;
	}

	// TODO: Load a mesh and store into vertices. Then write as a recursive method.
	// 1.	Start at root node
	// 2.	Find root's children nodes
	// 3.	Find children's children nodes, etc... (recursive)
	// 4.	For each node, find vertices
	// 5.	For each node, find indices
	// 6.	Populate list of vertices for each node

	// Search nodes
	const aiNode* ROOT_NODE = scene->mRootNode;

	// Search meshes of node (starting from root node)
	for (int i = 0; i < ROOT_NODE->mNumMeshes; i++)
	{
		// Get mesh from scene
		unsigned int meshIndex = ROOT_NODE->mMeshes[i];
		aiMesh* nodeMesh = scene->mMeshes[meshIndex];

		// Extract mesh's positions and normals
		for (int v = 0; v < nodeMesh->mNumVertices; v++)
		{
			glm::vec3 position;
			glm::vec3 normal;

			aiVector3D nodePosition = nodeMesh->mVertices[v];
			position.x = nodePosition.x;
			position.y = nodePosition.y;
			position.z = nodePosition.z;

			aiVector3D nodeNormal = nodeMesh->mNormals[v];
			normal.x = nodeNormal.x;
			normal.y = nodeNormal.y;
			normal.z = nodeNormal.z;

			// Append to vertices
			Vertex newVertex;
			newVertex.position = position;
			newVertex.normal = normal;

			this->vertices.push_back(newVertex);
		}
	}

	// Search children nodes, and repeat above step for each one

}

void Mesh::processNode()
{
}

void Mesh::draw()
{
}

