cmake_minimum_required(VERSION 3.10.0)
project("model-viewer" VERSION 0.1.0 LANGUAGES C CXX)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/")

add_executable(${PROJECT_NAME} 
    "src/main.cpp"
    "src/shader.cpp"
    "src/camera.cpp"
)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE glad::glad
    PRIVATE glfw
    PRIVATE glm::glm
)

add_custom_target(
    copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/shaders"
    "${PROJECT_BINARY_DIR}/shaders"
    COMMENT "Copying shader files into binary directory..."
)

add_dependencies(${PROJECT_NAME} copy_resources)