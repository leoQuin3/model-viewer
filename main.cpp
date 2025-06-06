#include <iostream>
#include <GLFW/glfw3.h>

// TODO: test out configurations of CMakeLists and vcpkg for development!

int main(int, char**){
    if (!glfwInit())
	{
        std::cout << "Failed to initialize glfw.\n";
		return EXIT_FAILURE;
	}
    // FIXME: nothing outputs when running...
    std::cout << "glfw success!\n";

    glfwTerminate();
    return EXIT_SUCCESS;
}
