# Set the minimum required version of CMake that the project can use.
# This ensures that the version of CMake used to build the project has
# all the features that are required.
cmake_minimum_required(VERSION 3.15)

# Declare a new project named 'example_1'. This sets various variables
# related to the project and provides a context for the following CMake
# commands.
project(GeometricTools)

add_library(GeometricTools INTERFACE GeometricTools.h)
add_library(Framework::GeometricTools ALIAS GeometricTools)
target_include_directories(GeometricTools INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Search for the OpenGL package on the system. The REQUIRED argument
# ensures that if OpenGL is not found, CMake will produce an error and
# stop processing. If found, this command sets up various variables
# and imported targets related to OpenGL that can be used later.
find_package(OpenGL REQUIRED)


target_link_libraries(GeometricTools INTERFACE glm glfw glad OpenGL::GL stb)