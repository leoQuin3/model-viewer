#include <iostream>
#include <GLFW/glfw3.h>

/* ********************************
GOAL: Render something
TODO:
    1) Triangle test
    2) Load and render model
******************************** */ 

int main(int, char**)
{
    if (!glfwInit())
	{
        std::cout << "Failed to initialize glfw.\n";
		return EXIT_FAILURE;
	}
    std::cout << "glfw success!\n";

    glfwTerminate();
    return EXIT_SUCCESS;
}
