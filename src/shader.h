#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
public:
	GLuint programID;

    Shader(std::string VERTEX_SRC_PATH, std::string FRAGMENT_SRC_PATH);
	~Shader();
	
	void use();


private:
	void printShaderCompileStatus(GLuint id, std::string name);
	void printProgramCompileStatus(GLuint id, std::string name);
};