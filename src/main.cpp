#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

/*
    TODO: Draw 3D models
    -   Create camera class
    -   3D cube test
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

    // Create shader program
    Shader shaderProgram("shaders/vtx_shader.glsl", "shaders/frag_shader.glsl");
    shaderProgram.use();

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Transformations
        glm::mat4 rotateMatrix = glm::mat4(1.0);
        rotateMatrix = glm::rotate(rotateMatrix, glm::radians(static_cast<float>(glfwGetTime()) * 100.f), glm::vec3(glm::cos(glfwGetTime()), 0.f, glm::sin(glfwGetTime())));
        
        // Assign uniform
        shaderProgram.assignMat4("rotateMat", rotateMatrix, GL_FALSE);

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
