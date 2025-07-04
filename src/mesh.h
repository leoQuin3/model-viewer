#include <glm/glm.hpp>
#include <vector>

/*
	TODO: Create Mesh class that loads a mesh and stores vertices.
	INPUT: A file
	OUTPUT: vertices and indices
*/

// Consecutive set of attributes.
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
};

// A data structure which holds data ready for buffering in OpenGL.
class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Mesh(const std::string FILE_PATH);

	void draw();

private:
	void processNode();
};