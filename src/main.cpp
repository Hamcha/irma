#include "Player.h"

int main(int argc, char *argv[]) {
	// Player properties
	const int w = 800, h = 600;

	// OpenGL context attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error initializing SDL", SDL_GetError(), NULL);
		SDL_Quit();
		exit(1);
	}

	SDL_Window* window = SDL_CreateWindow("Irma", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (window == NULL) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error opening window", SDL_GetError(), NULL);
		SDL_Quit();
		exit(1);
	}

	Player player;

	try {
		player.InitContext(window, w, h);
	} catch (const PlayerException& e) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Player initialization error", e.what(), window);
		SDL_Quit();
		return 1;
	}

	try {
		player.LoadDirectory("projects/example");
	} catch (const ShaderException& e) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, e.type == SHADER_COMPILE_FAILURE ? "Compilation error" : "Link error", e.what(), window);
		SDL_Quit();
		return 1;
	}

	player.MakeUniforms();

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

		player.Loop();
	}

	player.Dispose();
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}