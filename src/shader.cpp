#include "shader.h"

Shader::Shader(std::string vertex_src_path, std::string fragment_src_path)
{
	// Create program
	this->programID = glCreateProgram();

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
    glAttachShader(this->programID, vertexShader);
    glAttachShader(this->programID, fragmentShader);
    glLinkProgram(this->programID);
    printProgramCompileStatus(this->programID, "SHADER_PROGRAM");

    // Clean up shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(this->programID);
}

void Shader::use()
{
    glUseProgram(this->programID);
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
    else
        std::clog << "Info::Shader:: \"" << name << "\" ::Compilation success!\n";
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
    else
        std::clog << "Info::Shader_Program:: \"" << name << "\" ::Linking success!\n";
}
