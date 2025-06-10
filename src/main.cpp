#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

/*
TODO: Render something
    - Triangle test
*/ 

int main(int, char**)
{
    if (!glfwInit())
	{
        std::cout << "ERROR: Failed to initialize glfw.\n";
		return EXIT_FAILURE;
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Model Viewer", NULL, NULL);
    if (!window)
    {
        std::cout << "ERROR: Failed to create window.\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR::GLAD: Failed to initialize OpenGL context.\n";
        return EXIT_FAILURE;
    }

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    std::cout << "success!\n";

    glfwTerminate();
    return EXIT_SUCCESS;
}
