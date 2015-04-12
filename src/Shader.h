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
	GLint GetUniform(std::string name);

public:
	std::map<std::string, BaseUniform> uniforms;

	Shader();
	void SetShader(const char* shaderSource);
	void Render(GLuint texture, int w, int h, int scale = 1);
	void Draw(int w, int h, int scale = 1);
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