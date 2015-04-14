#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <stdexcept>
#include <string>
#include <vector>
#include "Shader.h"
#include "LuaScript.h"

class Player {
private:
	SDL_Window* window;
	GLuint texture;
	int width, height;

protected:
	SDL_GLContext context;

public:
	Shader userShader, appShader;
	LuaScript script;

	~Player();
	void Init(const int w, const int h, const char* name);
	void SetShader(const std::string shaderSrc);
	void MakeUniforms();
	void Loop();
};

class PlayerException : public std::runtime_error {
public:
	PlayerException(std::string what) : std::runtime_error(what.c_str()) {}
};
