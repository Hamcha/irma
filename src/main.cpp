#include <SDL.h>

int main(int argc, char *argv[]) {
	int errcode = 0;
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error initializing SDL", SDL_GetError(), NULL);
		errcode = 1;
		goto quit;
	}

	SDL_Window *window = SDL_CreateWindow("Irma", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error opening window", SDL_GetError(), NULL);
		errcode = 1;
		goto quit;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (!context) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error creating OpenGL context", SDL_GetError(), NULL);
		errcode = 1;
		goto quit;
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);

quit:	
	SDL_Quit();
	return errcode;
}