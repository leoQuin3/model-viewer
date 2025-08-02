#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "model.h"

/*
    TODO: Create mirror using stencil testing
        -   Write to stencil buffer
        -   Draw reflection in the mirror
*/

// Window and cursor properties
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

float lastMouseX = WINDOW_WIDTH / 2.;
float lastMouseY = WINDOW_HEIGHT / 2.;
bool cursorFirstEntered = true;

// Camera
const float MAX_ELEVATION = 89.99f;
const float MIN_ELEVATION = -89.99f;
const float MAX_RADIUS = 25.f;
const float MIN_RADIUS = .5f;
const float DEFAULT_RADIUS = 10.f;
float radius = DEFAULT_RADIUS;
float elevationAngle = 0.f;
float azimuthAngle = 0.f;

const glm::vec3 WORLD_ORIGIN(0);
glm::vec3 panOffset(0.f);

Camera camera = Camera(WORLD_ORIGIN + panOffset, glm::vec3(1, 0, -1), glm::vec3(0, 1, 0));

// Prototypes
void orbit_camera(const float theta, const float phi, const glm::vec3 target);
void assign_transforms(Shader &shader, float scale, glm::vec3 position);

// Callback functions
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

int main(int, char **)
{
    // Initialize OpenGL
    if (!glfwInit())
    {
        std::cout << "ERROR::Failed to initialize glfw.\n";
        return EXIT_FAILURE;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Model Viewer", NULL, NULL);
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

    // Register callback functions
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    // 3D Bunny
    Model model("assets/StanfordBunny.glb");

    // Mirror Model
    Mesh mirror(

        // Vertices
        std::vector<Vertex>
        {
            {glm::vec3(-2.5f, -2.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
            {glm::vec3( 2.5f, -2.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
            {glm::vec3( 2.5f,  2.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
            {glm::vec3(-2.5f,  2.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)}
        },

        // Indices
        {
            0, 1, 2, 2, 3, 0
        }
    );

    // Create shader program
    Shader modelShader("shaders/vtx_shader.glsl", "shaders/frag_shader.glsl");

    // Enable properties
    camera.mouseSensitivity = 0.005f;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  // Replace values with 1 where mirror is drawn
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Activate shader program
    modelShader.use();
    
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        // Update camera
        orbit_camera(elevationAngle, azimuthAngle, WORLD_ORIGIN + panOffset);
        
        // TODO: Create a reflection using stencil buffer
        glStencilFunc(GL_ALWAYS, 1, 0xFF);  // Pass all fragments
        glDisable(GL_DEPTH_TEST);
        assign_transforms(modelShader, 1.f, glm::vec3(0));
        mirror.draw();
        
        // TEST: Draw rabbit "inside" mirror
        glStencilFunc(GL_EQUAL, 1, 0xFF);    // Draw only where 1
        glEnable(GL_DEPTH_TEST);
        assign_transforms(modelShader, 1.f, glm::vec3(0, 0, -3));
        model.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout << "success!\n";

    glfwTerminate();
    return EXIT_SUCCESS;
}

// Rotate camera by dragging
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    float xOffset = static_cast<float>(xpos) - lastMouseX;
    float yOffset = lastMouseY - static_cast<float>(ypos);

    lastMouseX = static_cast<float>(xpos);
    lastMouseY = static_cast<float>(ypos);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        // Panning
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            panOffset -= (camera.getUp() * yOffset + camera.getRight() * xOffset) * camera.mouseSensitivity;
        }

        // Rotation
        else
        {
            azimuthAngle -= xOffset * camera.mouseSensitivity;
            elevationAngle -= yOffset * camera.mouseSensitivity;

            if (elevationAngle >= glm::radians(MAX_ELEVATION))
                elevationAngle = glm::radians(MAX_ELEVATION);
            else if (elevationAngle <= glm::radians(MIN_ELEVATION))
                elevationAngle = glm::radians(MIN_ELEVATION);
        }
    }
}

// Zoom by scrolling
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    radius -= static_cast<float>(yoffset) * 0.5f;

    if (radius > MAX_RADIUS)
        radius = MAX_RADIUS;
    else if (radius < MIN_RADIUS)
        radius = MIN_RADIUS;
}

// Reset to default position
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_R)
    {
        elevationAngle = 0.f;
        azimuthAngle = 0.f;
        radius = DEFAULT_RADIUS;
        panOffset = glm::vec3(0);
    }
}

// Set camera position and orientation
void orbit_camera(const float theta, const float phi, const glm::vec3 target)
{
    camera.position.x = radius * glm::cos(theta) * glm::sin(phi);
    camera.position.y = radius * glm::sin(theta);
    camera.position.z = radius * glm::cos(theta) * glm::cos(phi);

    camera.position += target;

    camera.lookAtPosition(target);
}

// Assign vertex transformations to shader program
void assign_transforms(Shader &shader, float scale, glm::vec3 position)
{
    glm::mat4 modelMat = glm::mat4(1.0);
    modelMat = glm::scale(modelMat, glm::vec3(scale));

    glm::mat4 worldMat = glm::mat4(1.0);
    worldMat = glm::translate(worldMat, position);

    glm::mat4 viewMat = camera.getViewMatrix();

    glm::mat4 projectionMat = glm::mat4(1.0);
    projectionMat = glm::perspective(glm::radians(45.f), static_cast<float>(WINDOW_WIDTH) / (WINDOW_HEIGHT), 0.1f, 100.0f);

    shader.assignMat4("modelMat", modelMat, GL_FALSE);
    shader.assignMat4("worldMat", worldMat, GL_FALSE);
    shader.assignMat4("viewMat", viewMat, GL_FALSE);
    shader.assignMat4("projectionMat", projectionMat, GL_FALSE);
}

// NOTE: Kept here for reference
// void draw_outlined(Model &model, float outlineScale, Shader &modelShader, Shader &outlineShader)
// {
//     // Draw model
//     // -----------------------------------------------------------------------
//     glEnable(GL_STENCIL_TEST);  // Allow writing to stencil buffer
//     glStencilFunc(GL_ALWAYS, 1, 0xFF);  // Pass all fragments

//     modelShader.use();
//     assign_transforms(modelShader);
//     model.draw();

//     // Draw outline
//     // -----------------------------------------------------------------------
//     glStencilFunc(GL_NOTEQUAL, 1, 0xFF);    // Only draw where stencil value
//                                             // is NOT 1 (i.e. anywhere except
//                                             // where the rabbit is drawn).
//     glDisable(GL_DEPTH_TEST);   // Draw on top of everything
    
//     outlineShader.use();
//     assign_transforms(outlineShader);
//     outlineShader.assignFloat("outlineScale", outlineScale);
//     model.draw();

//     glEnable(GL_DEPTH_TEST);    // Re-enable depth testing
// }