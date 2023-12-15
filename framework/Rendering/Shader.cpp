#include "Shader.h"
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

Shader::Shader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc)
{
    // Compile vertex shader, fragment shader
    VertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSrc.c_str());
    FragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc.c_str());

    // Create shader program and attach compiled shaders
    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    GLint success;
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logSize;
        glGetProgramiv(ShaderProgram, GL_INFO_LOG_LENGTH, &logSize);
        std::vector<GLchar> infoLog(logSize);
        glGetProgramInfoLog(ShaderProgram, logSize, NULL, infoLog.data());

        std::cerr << "Shader program linking failed: " << infoLog.data() << std::endl;
    }
    else {
        // Successful linking, no need to call glUseProgram(ShaderProgram) here.
        glDeleteShader(VertexShader);
        glDeleteShader(FragmentShader);
    }
}


Shader::~Shader()
{
    glDeleteProgram(ShaderProgram);
}


void Shader::Bind() const
{
    glUseProgram(ShaderProgram);
}


void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::UploadUniformFloat1(const std::string& name, const GLfloat number) {
    GLuint location = glGetUniformLocation(ShaderProgram, name.c_str());
    glUniform1f(location, number);
}

void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& vector)
{
    GLuint location = glGetUniformLocation(ShaderProgram, name.c_str());
    glUniform2f(location, vector.x, vector.y);
}

void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& vector)
{
    GLuint location = glGetUniformLocation(ShaderProgram, name.c_str());
    glUniform3f(location, vector[0], vector[1], vector[2]);
}

void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& vector)
{
    GLuint location = glGetUniformLocation(ShaderProgram, name.c_str());
    glUniform4f(location, vector[0], vector[1], vector[2], vector[3]);
}

void Shader::UploadUniformMatrix4fv(const std::string& name, const glm::mat4& matrix) {
    GLuint location = glGetUniformLocation(ShaderProgram, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::UploadUniform1i(const std::string& name, const GLuint slot) {
    GLuint location = glGetUniformLocation(ShaderProgram, name.c_str());
    glUniform1i(location, slot);
}

GLuint Shader::CompileShader(GLenum shaderType, const char * shaderSrc)
{
    auto shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSrc , nullptr);
    glCompileShader(shader);
    return shader;
}
