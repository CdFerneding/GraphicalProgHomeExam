#ifndef PROG2002_GLFWAPPLICATION_H
#define PROG2002_GLFWAPPLICATION_H
#define GLFW_INCLUDE_NONE

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <string>
#include <iostream>

class GLFWApplication
{
public:
    explicit GLFWApplication(const std::string &name = "GLFWApplication", const std::string &version = "0.0.1", unsigned int width = 800, unsigned int height = 600);
    ~GLFWApplication();

    //
    // Initialization that can be predefined
    //
    virtual unsigned Init(); // Virtual function with default behavior.


    // 
    // Run function that is implemented individually
    //
    virtual unsigned Run() = 0; // Pure virtual function that must be redefined.


    // 
    // stop funciton that can be predefined
    //
    virtual unsigned stop();

protected:
    //...other functions...
    const std::string &name;
    const std::string &version;
    GLFWwindow* window;
    GLuint program;
    unsigned width;
    unsigned height;
};



#endif //PROG2002_GLFWAPPLICATION_H
