#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

// A set of attributes that define a vertex.
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
};

// A data structure which holds vertices ready to be drawn by OpenGL.
class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~Mesh();

	void draw();

private:
	GLuint VAO, VBO, EBO;
	void initializeBuffers();
};