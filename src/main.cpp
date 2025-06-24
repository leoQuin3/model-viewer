#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

/*
    TODO:
    -   Create camera
*/

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

unsigned int getVertCount(unsigned int arrLength, unsigned int stride);

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

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Model Viewer", NULL, NULL);
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

    // Cube coordinates (NDC)
    float vertices[] = {
        // X, Y, Z             // R, G, B
        -0.5f, -0.5f,  0.5f,   1.f, 0.f, 0.f,
         0.5f, -0.5f,  0.5f,   1.f, 0.f, 0.f,
         0.5f,  0.5f,  0.5f,   1.f, 0.f, 0.f,
         0.5f,  0.5f,  0.5f,   1.f, 0.f, 0.f,
        -0.5f,  0.5f,  0.5f,   1.f, 0.f, 0.f,
        -0.5f, -0.5f,  0.5f,   1.f, 0.f, 0.f,

        -0.5f, -0.5f, -0.5f,   0.f, 1.f, 0.f,
        -0.5f,  0.5f, -0.5f,   0.f, 1.f, 0.f,
         0.5f,  0.5f, -0.5f,   0.f, 1.f, 0.f,
         0.5f,  0.5f, -0.5f,   0.f, 1.f, 0.f,
         0.5f, -0.5f, -0.5f,   0.f, 1.f, 0.f,
        -0.5f, -0.5f, -0.5f,   0.f, 1.f, 0.f,

        -0.5f, -0.5f, -0.5f,   0.f, 0.f, 1.f,
        -0.5f, -0.5f,  0.5f,   0.f, 0.f, 1.f,
        -0.5f,  0.5f,  0.5f,   0.f, 0.f, 1.f,
        -0.5f,  0.5f,  0.5f,   0.f, 0.f, 1.f,
        -0.5f,  0.5f, -0.5f,   0.f, 0.f, 1.f,
        -0.5f, -0.5f, -0.5f,   0.f, 0.f, 1.f,

         0.5f, -0.5f, -0.5f,   1.f, 0.f, 1.f,
         0.5f,  0.5f, -0.5f,   1.f, 0.f, 1.f,
         0.5f,  0.5f,  0.5f,   1.f, 0.f, 1.f,
         0.5f,  0.5f,  0.5f,   1.f, 0.f, 1.f,
         0.5f, -0.5f,  0.5f,   1.f, 0.f, 1.f,
         0.5f, -0.5f, -0.5f,   1.f, 0.f, 1.f,

        -0.5f,  0.5f, -0.5f,   1.f, 1.f, 0.f,
        -0.5f,  0.5f,  0.5f,   1.f, 1.f, 0.f,
         0.5f,  0.5f,  0.5f,   1.f, 1.f, 0.f,
         0.5f,  0.5f,  0.5f,   1.f, 1.f, 0.f,
         0.5f,  0.5f, -0.5f,   1.f, 1.f, 0.f,
        -0.5f,  0.5f, -0.5f,   1.f, 1.f, 0.f,

        -0.5f, -0.5f, -0.5f,   1.f, 1.f, 1.f,
         0.5f, -0.5f, -0.5f,   1.f, 1.f, 1.f,
         0.5f, -0.5f,  0.5f,   1.f, 1.f, 1.f,
         0.5f, -0.5f,  0.5f,   1.f, 1.f, 1.f,
        -0.5f, -0.5f,  0.5f,   1.f, 1.f, 1.f,
        -0.5f, -0.5f, -0.5f,   1.f, 1.f, 1.f,
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

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Vertex Transformations
        glm::mat4 modelMat = glm::mat4(1.0);
        modelMat = glm::rotate(
            modelMat,
            glm::radians(static_cast<float>(glfwGetTime()) * 100.f),
            glm::vec3(glm::cos(glfwGetTime()), 0.f, glm::sin(glfwGetTime()))
        );
        glm::mat4 worldMat = glm::mat4(1.0);
        worldMat = glm::translate(
            worldMat,
            glm::vec3(glm::cos(glfwGetTime()), 0, -3)
        );
        glm::mat4 projectionMat = glm::mat4(1.0);
        projectionMat = glm::perspective(
            glm::radians(45.f),
            static_cast<float>(WINDOW_WIDTH) / (WINDOW_HEIGHT),
            0.1f, 100.0f
        );

        // Assign uniforms
        shaderProgram.assignMat4("modelMat", modelMat, GL_FALSE);
        shaderProgram.assignMat4("worldMat", worldMat, GL_FALSE);
        shaderProgram.assignMat4("projectionMat", projectionMat, GL_FALSE);

        // Draw object
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, getVertCount(sizeof(vertices) / sizeof(float), 6));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout << "success!\n";

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return EXIT_SUCCESS;
}

unsigned int getVertCount(unsigned int arrLength, unsigned int stride)
{
    return arrLength / stride;
}
