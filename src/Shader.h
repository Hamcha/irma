#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <map>
#include "Quad.h"

class Shader {
private:
	GLint program, vertexShader, fragmentShader;
	GLuint vbo, ibo, vertAttrib, fbo, rb;
	Quad quad;

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

class ShaderLinkException {
private:
	std::string reason;
public:
	ShaderLinkException(std::string what);
	std::string what();
};

#endif