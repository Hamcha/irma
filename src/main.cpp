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

	SDL_Window *window = SDL_CreateWindow("Irma", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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

	// Viewport and projection
	GLint m_viewport[4];
	int w, h;
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	w = m_viewport[2];
	h = m_viewport[3];
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

	// OpenGL Features
	glEnable(GL_TEXTURE_2D); 
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
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