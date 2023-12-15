#ifndef PROG2002_SHADER_H
#define PROG2002_SHADER_H

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
	~Shader();

	void Bind() const;
	void Unbind() const;
	void UploadUniformFloat1(const std::string& name, const GLfloat number);
	void UploadUniformFloat2(const std::string& name, const glm::vec2& vector);
	void UploadUniformFloat3(const std::string& name, const glm::vec3& vector);
	void UploadUniformFloat4(const std::string& name, const glm::vec4& vector);
	void UploadUniformMatrix4fv(const std::string& name, const glm::mat4& matrix);

	void UploadUniform1i(const std::string& name, const GLuint slot);

private:
	GLuint VertexShader;
	GLuint FragmentShader;
	GLuint ShaderProgram; 
	GLuint CompileShader(GLenum shaderType, const char * shaderSrc);
};

#endif //PROG2002_SHADER_H