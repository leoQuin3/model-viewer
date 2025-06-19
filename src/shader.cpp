#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

Shader::Shader(std::string vertex_src_path, std::string fragment_src_path)
{
	// Create program
	this->id = glCreateProgram();

    // Get shader source code
    std::string vtxCode, fragCode;
    std::ifstream vtxFile, fragFile;

    vtxFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // Read files
        vtxFile.open(vertex_src_path);
        fragFile.open(fragment_src_path);

        std::stringstream vtxStream, fragStream;
        
        vtxStream << vtxFile.rdbuf();
        fragStream << fragFile.rdbuf();

        vtxFile.close();
        fragFile.close();

        vtxCode = vtxStream.str();
        fragCode = fragStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_READ_NOT_SUCCESSFUL: " << e.what() << "\n";
    }

    // Compile shaders
    GLuint vertexShader, fragmentShader;
    const char* c_vertexCode = vtxCode.c_str();
    const char* c_fragmentCode = fragCode.c_str();

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &c_vertexCode, NULL);
    glCompileShader(vertexShader);
    printShaderCompileStatus(vertexShader, "vertex shader");

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &c_fragmentCode, NULL);
    glCompileShader(fragmentShader);
    printShaderCompileStatus(fragmentShader, "fragment shader");

    // Create shader program
    glAttachShader(this->id, vertexShader);
    glAttachShader(this->id, fragmentShader);
    glLinkProgram(this->id);
    printProgramCompileStatus(this->id, "SHADER_PROGRAM");

    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(this->id);
}

void Shader::use()
{
    glUseProgram(this->id);
}

void Shader::assignMat4(const std::string uniformName, const glm::mat4 &mat4, GLboolean transpose)
{
    const GLchar* unifName_c = uniformName.c_str();
    glUniformMatrix4fv(glGetUniformLocation(this->id, unifName_c), 1, transpose, glm::value_ptr(mat4));
}

void Shader::printShaderCompileStatus(GLuint id, std::string name)
{
    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_ERROR: \"" << name << "\"\n" << infoLog << std::endl;
    }
}

void Shader::printProgramCompileStatus(GLuint id, std::string name)
{
    int success;
    char infoLog[512];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER_PROGRAM::LINKING_ERROR: \"" << name << "\"\n" << infoLog << std::endl;
    }
}
