#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <map>

class Shader {
private:
	GLint program, vertexShader, fragmentShader, fbo;

public:
	std::map<std::string, GLfloat> uniforms1f;
	std::map<std::string, GLint  > uniforms1i;

	Shader();
	void SetShader(const char* shaderSource);
	void Render(GLuint texture, int w, int h, int scale = 1);
};

class ShaderCompilationException {
private:
	std::string reason;
public:
	ShaderCompilationException(std::string what);
	std::string what();
};

#endif