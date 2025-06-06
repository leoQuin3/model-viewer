cmake_minimum_required(VERSION 3.10.0)
project(model-viewer VERSION 0.1.0 LANGUAGES C CXX)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/")

add_executable(${PROJECT_NAME} main.cpp)

find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)