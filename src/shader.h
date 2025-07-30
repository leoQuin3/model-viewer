#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
public:
	GLuint id;

    Shader(std::string VERTEX_SRC_PATH, std::string FRAGMENT_SRC_PATH);
	~Shader();
	
	void use();
	void assignMat4(const std::string &uniformName, const glm::mat4 &mat4, GLboolean transpose);
	void assignFloat(const std::string &uniformName, float value);

private:
	void printShaderCompileStatus(GLuint id, std::string name);
	void printProgramCompileStatus(GLuint id, std::string name);
};