# Set the minimum required version of CMake that the project can use.
# This ensures that the version of CMake used to build the project has
# all the features that are required.
cmake_minimum_required(VERSION 3.15)

# Declare a new project named 'tuto_lab1_glfw'. This sets various variables
# related to the project and provides a context for the following CMake
# commands.
project(assignment)
# Add an executable target named 'tuto_lab1_glfw' that is built from the source
# file 'main.cpp'. This means that CMake will generate build rules to compile
# 'main.cpp' and link it into an executable program named 'tuto_lab1_glfw'.
add_executable(assignment src/main.cpp src/assignment.cpp src/assignment.h "src/shaders/grid.h" "src/shaders/square.h" "src/shaders/unit.h")

# Specify libraries to link with the 'tuto_lab1_glfw' target.
# This tells CMake that when it's linking 'tuto_lab1_glfw', it should also
# link the following libraries:
# - glfw: This is a windowing library commonly used with OpenGL.
# - glad: A library to load OpenGL extensions.
# - OpenGL::GL: This is an imported target for the main OpenGL library
#               provided by the find_package(OpenGL) command.
target_link_libraries(${PROJECT_NAME} PRIVATE Framework::GLFWApplication Framework::Rendering glfw glad OpenGL::GL stb Framework::GeometricTools)

# Define a preprocessor macro for the STB image library
target_compile_definitions(${PROJECT_NAME}
  PRIVATE
  STB_IMAGE_IMPLEMENTATION)

# Set up a compile definition for the texture directory
# Escaping the path ensures it's recognized correctly by the preprocessor
# This makes it easier to use absolute paths in the source code for texture loading
set(TEXTURES_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures")
string(REPLACE "/" "\/" ESCAPED_TEXTURES_PATH ${TEXTURES_PATH})
# "this compile definition is exquivalent to having for example:
# #define TEXTURES_DIR "/home/whatever/labs/build/bin/resources/textures/""
target_compile_definitions(${PROJECT_NAME} PUBLIC TEXTURES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/")


# Custom command to copy the 'cat.png' texture to the expected build directory after the build
# This ensures that the texture is available at runtime, regardless of where the executable is invoked from

# Custom command to copy all files from the 'textures' directory
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures)
