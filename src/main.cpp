#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

// Triangle Test
/*
    TODO:
    -   Abstract boiler plate
    -   Create classes for shaders
*/

int main(int, char**)
{
    if (!glfwInit())
	{
        std::cout << "ERROR::Failed to initialize glfw.\n";
		return EXIT_FAILURE;
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Model Viewer", NULL, NULL);
    if (!window)
    {
        std::cout << "ERROR::Failed to create window.\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR::GLAD::Failed to initialize OpenGL context.\n";
        return EXIT_FAILURE;
    }

    // Triangle coordinates (NDC)
    float vertices[] = {
        // X, Y, Z          // R, G, B
        -.5f, -.5f, 0.f,    1.f, 0.f, 0.f,
         .5f, -.5f, 0.f,    0.f, 1.f, 0.f,
         0.f,  .5f, 0.f,    0.f, 0.f, 1.f,
    };

    // Buffers
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Open files
    std::ifstream vtx_sdr_file, frag_sdr_file;
    vtx_sdr_file.open("shaders/vtx_shader.glsl");
    if (!vtx_sdr_file.is_open())
    {
        std::cout << "ERROR::IFSTREAM::Failed to open shader file\n";
        return EXIT_FAILURE;
    }
    frag_sdr_file.open("shaders/frag_shader.glsl");
    if (!frag_sdr_file.is_open())
    {
        std::cout << "ERROR::IFSTREAM::Failed to open shader file\n";
        return EXIT_FAILURE;
    }

    // Get shader source code
    std::stringstream vtx_src_buffer;
    vtx_src_buffer << vtx_sdr_file.rdbuf();
    std::string vtx_src_str = vtx_src_buffer.str();
    const char* vtx_code = vtx_src_str.c_str();
    vtx_sdr_file.close();

    std::stringstream frag_src_buffer;
    frag_src_buffer << frag_sdr_file.rdbuf();
    std::string frag_src_str = frag_src_buffer.str();
    const char* frag_code = frag_src_str.c_str();
    frag_sdr_file.close();

    // Create shaders
    GLuint vtx_shader, frag_shader;
    int success;
    char infoLog[512];

    vtx_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vtx_shader, 1, &vtx_code, NULL);
    glCompileShader(vtx_shader);
    glGetShaderiv(vtx_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vtx_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    std::cout << "INFO::Vertex Shader compilation success!\n";

    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_code, NULL);
    glCompileShader(frag_shader);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(frag_shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    std::cout << "INFO::Fragment Shader compilation success!\n";

    // Create shader program
    GLuint shaderProgram;

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vtx_shader);
    glAttachShader(shaderProgram, frag_shader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    std::cout << "Shader linking success!\n";

    // Clean up shader
    glDeleteShader(vtx_shader);
    glDeleteShader(frag_shader);

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout << "success!\n";

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return EXIT_SUCCESS;
}
