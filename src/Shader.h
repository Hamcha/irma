#ifndef _SHADER_H
#define _SHADER_H

#include <GL/glew.h>
#include <string>
#include <map>
#include "Quad.h"
#include "GLUtils.h"

class Shader {
private:
	GLint program, vertexShader, fragmentShader;
	GLuint vbo, ibo, vertAttrib, fbo, rb;
	Quad quad;
	std::map<std::string, GLint> uniformCache;

protected:
	GLint GetUniform(const std::string name);

public:
	std::map<std::string, BaseUniform*> uniforms;

	Shader();
	void SetShader(const char* shaderSource);
	void Render(const GLuint texture, const int w, const int h, const int scale = 1);
	void Draw();
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