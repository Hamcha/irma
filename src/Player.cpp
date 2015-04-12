#include "Player.h"
#include <fstream>

const GLchar appShaderSrc[] =
{
	"#version 140\n"
	"out vec4 LFragment;"
	"uniform vec2 uResolution;"
	"uniform sampler2D texture;"
	""
	"void main() {"
	"	vec2 uv = gl_FragCoord.xy / uResolution.xy;"
	"	LFragment = texture2D(texture, uv);"
	"}"
};

void Player::InitContext(SDL_Window* win, const int w, const int h) {
	SDL_GLContext context = SDL_GL_CreateContext(win);
	if (context == NULL) {
		throw new PlayerException(std::string(SDL_GetError()));
	}

	glewExperimental = GL_TRUE;
	GLenum g = glewInit();
	if (g != GLEW_OK) {
		throw new PlayerException(std::string((const char*) glewGetErrorString(g)));
	}
	SDL_GL_SetSwapInterval(1);

	// Clear screen color: Black
	glClearColor(0.f, 0.f, 0.f, 0.f);

	// Generate render texture
	glGenTextures(1, &texture);

	width = w;
	height = h;
	window = win;

	userShader.Init();
	appShader.Init();
	appShader.SetShader(appShaderSrc);
}

void Player::LoadDirectory(const std::string dirname) {
	// Read shader file
	std::ifstream shaderFile(dirname + "/shader.glsl"); //TODO Check for existence
	std::string shaderSrc = "", line;
	while (!shaderFile.eof()) {
		std::getline(shaderFile, line);
		shaderSrc += line + "\n";
	}
	shaderFile.close();

	// Create shaders
	userShader.SetShader(shaderSrc.c_str());
}

void Player::MakeUniforms() {
	Uniform<GLVec2f>* resolution = new Uniform<GLVec2f>(GLVec2f{ width, height });
	userShader.uniforms["uResolution"] = resolution;
	appShader.uniforms["uResolution"] = resolution;

	Uniform<GLVec2i>* mouseCoord = new Uniform<GLVec2i>(GLVec2i{ 0, 0 });
	userShader.uniforms["uMouseCoord"] = mouseCoord;

	Uniform<GLfloat>* time = new Uniform<GLfloat>(0);
	userShader.uniforms["uTime"] = time;

	Uniform<Sampler2D>* renderTexture = new Uniform<Sampler2D>(texture);
	renderTexture->texid = 0;
	appShader.uniforms["texture"] = renderTexture;
}

void Player::Loop() {
	// Get input for uniforms
	Uniform<GLVec2i>* mouseCoord = (Uniform<GLVec2i>*)userShader.uniforms["uMouseCoord"];
	SDL_GetMouseState(&mouseCoord->value.x, &mouseCoord->value.y);

	Uniform<GLfloat>* time = (Uniform<GLfloat>*)userShader.uniforms["uTime"];
	time->value = SDL_GetTicks() / 1000.f;

	glClear(GL_COLOR_BUFFER_BIT);

	// Render shaders
	userShader.Render(texture, width, height);
	appShader.Draw();

	SDL_GL_SwapWindow(window);
}

void Player::Dispose() {
	SDL_GL_DeleteContext(context);
}