#ifndef ERRORHANDLING_H_
#define ERRORHANDLING_H_

#include <glad/glad.h>
#include <iostream>

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall() {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
		return false;
	}
	return true;
}

#endif //ERRORHANDLING_H