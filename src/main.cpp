#include <SDL.h>
#include <fstream>
#include "Shader.h"

int main(int argc, char *argv[]) {
	int errcode = 0;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error initializing SDL", SDL_GetError(), NULL);
		SDL_Quit();
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Irma", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error opening window", SDL_GetError(), NULL);
		SDL_Quit();
		return 1;
	}

	// OpenGL context attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (!context) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error creating OpenGL context", SDL_GetError(), window);
		SDL_Quit();
		return 1;
	}

	// Setup OpenGL and GLEW
	glEnable(GL_TEXTURE_2D);
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

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

	// Generate render texture
	GLuint texture;
	glGenTextures(1, &texture);

	Shader shader;
	try {
		shader.SetShader(shaderSrc.c_str());
	} catch (ShaderCompilationException e) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Shader compilation error", e.what().c_str(), window);
	}

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

		// Render to texture!
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		shader.Render(texture, w, h, 1);

		// Bind texture and draw it fullscreen (Two triangles!)
		glBindTexture(GL_TEXTURE_2D, texture);
		glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-1, -1, 0);
		glTexCoord2f(1, 0);
		glVertex3f(1, -1, 0);
		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-1, 1, 0);
		glEnd();
		glFlush();

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}