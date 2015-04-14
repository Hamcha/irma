#pragma once

#include <GL/glew.h>
#include <string>
#include <map>
#include <stdexcept>
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

	~Shader();
	void Init();
	void SetShader(const std::string shaderSource, const std::string shaderName = "UNSET");
	void Render(const GLuint texture, const int w, const int h);
	void Draw();
};

enum ShaderExceptionType {
	SHADER_COMPILE_FAILURE,
	PROGRAM_LINK_FAILURE
};

class ShaderException : public std::runtime_error {
public:
	ShaderExceptionType type;
	std::string name;
	ShaderException(ShaderExceptionType _type, std::string _name, std::string what)
		: std::runtime_error(what.c_str())
		{ type = _type; name = _name; }
};

