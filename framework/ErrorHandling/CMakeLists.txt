cmake_minimum_required(VERSION 3.15)
project(ErrorHandling)

find_package(OpenGL REQUIRED)

add_library(ErrorHandling INTERFACE)
target_include_directories(ErrorHandling INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

