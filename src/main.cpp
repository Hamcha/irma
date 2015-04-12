#include <SDL.h>
#include <fstream>
#include "Shader.h"

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


int main(int argc, char *argv[]) {
	const int w = 800, h = 600;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error initializing SDL", SDL_GetError(), NULL);
		SDL_Quit();
		return 1;
	}

	// OpenGL context attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	SDL_Window *window = SDL_CreateWindow("Irma", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (window == NULL) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error opening window", SDL_GetError(), NULL);
		SDL_Quit();
		return 1;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (context == NULL) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error creating OpenGL context", SDL_GetError(), window);
		SDL_Quit();
		return 1;
	}

	// Setup OpenGL and GLEW
	glewExperimental = GL_TRUE;
	GLenum g = glewInit();
	if (g != GLEW_OK) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error initializing GLEW", (char*)glewGetErrorString(g), window);
		SDL_Quit();
		return 1;
	}
	SDL_GL_SetSwapInterval(1);

	// Clear screen color: Black
	glClearColor(0.f, 0.f, 0.f, 0.f);
	
	// Read shader file
	std::ifstream shaderFile("projects/example/shader.glsl");
	std::string shaderSrc = "", line;
	while (!shaderFile.eof()) {
		std::getline(shaderFile, line);
		shaderSrc += line + "\n";
	}
	shaderFile.close();
	
	// Create shaders
	Shader userShader, appShader;
	try {
		userShader.SetShader(shaderSrc.c_str());
		appShader.SetShader(appShaderSrc);
	} catch (ShaderCompilationException e) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Shader compilation error", e.what().c_str(), window);
		SDL_Quit();
		return 1;
	} catch (ShaderLinkException e) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Shader link error", e.what().c_str(), window);
		SDL_Quit();
		return 1;
	}

	// Generate render texture
	GLuint texture;
	glGenTextures(1, &texture);

	// Set default uniforms
	Uniform<GLVec2f> resolution(GLVec2f{ w, h });
	userShader.uniforms["uResolution"] = &resolution;
	appShader.uniforms["uResolution"] = &resolution;

	Uniform<GLVec2i> mouseCoord(GLVec2i{ 0, 0 });
	userShader.uniforms["uMouseCoord"] = &mouseCoord;

	Uniform<GLfloat> time(0);
	userShader.uniforms["uTime"] = &time;

	Uniform<Sampler2D> renderTexture(texture);
	renderTexture.texid = 0;
	appShader.uniforms["texture"] = &renderTexture;

	SDL_Event event;
	bool running = true;
	while (running) {
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			}
		}

		// Get input for uniforms
		SDL_GetMouseState(&mouseCoord.value.x, &mouseCoord.value.y);
		time.value = SDL_GetTicks() / 1000.f;

		glClear(GL_COLOR_BUFFER_BIT);

		// Render shaders
		userShader.Render(texture, w, h);
		appShader.Draw();

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}