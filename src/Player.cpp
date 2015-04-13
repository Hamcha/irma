#include "Player.h"
#include "FileUtils.h"

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

void Player::Init(const int w, const int h, const char* name) {
	width = w;
	height = h;

	window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (window == NULL) {
		throw new PlayerException(std::string(SDL_GetError()));
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);
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

	userShader.Init();
	appShader.Init();
	appShader.SetShader(appShaderSrc);
}

void Player::SetShader(const std::string shaderSrc) {
	userShader.SetShader(shaderSrc.c_str());
}

void Player::MakeUniforms() {
	float w = width, h = height;
	userShader.uniforms["uResolution"] = new Uniform<GLVec2f>(GLVec2f{ w, h });
	appShader.uniforms["uResolution"] = new Uniform<GLVec2f>(GLVec2f{ w, h });
	userShader.uniforms["uMouseCoord"] = new Uniform<GLVec2i>(GLVec2i{ 0, 0 });
	userShader.uniforms["uTime"] = new Uniform<GLfloat>(0);

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
	userShader.Dispose();
	appShader.Dispose();
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}