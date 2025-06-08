#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

/* ******************************************************************************************************************
GOAL 0: Figure how to make project building cross-platform. If you're reading this and want to fix it, please help :(
TODO:
    1) Fix linking errors with glad
    2) Use MinGW triplet for libraries (for now)???
    3) Maybe edit CMakePresets to have different presets of triplets/compilers for vcpkg???


GOAL 1: Render something
TODO:
    1) Triangle test
    2) Load and render model
****************************************************************************************************************** */

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
    {
        std::cout << "ERROR::GLAD: Failed to load glfw.\n";
        return EXIT_FAILURE;
    }
    std::cout << "success!\n";

    glfwTerminate();
    return EXIT_SUCCESS;
}
