#include "Project.h"
#include <fstream>

int main(int argc, char *argv[]) {
	// Player properties
	const int w = 1280, h = 720;

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

	Project project;
	Player* player;

	try {
		project.LoadDirectory("projects/example");
		player = project.CreatePlayer(w, h);

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

			player->Loop();
		}
	} catch (const PlayerException& e) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Player initialization error", e.what(), NULL);
		SDL_Quit();
		return 1;
	} catch (const ShaderException& e) {
		std::string errname = e.type == SHADER_COMPILE_FAILURE ? "Compilation error - " + e.name : "Link error";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, errname.c_str(), e.what(), NULL);
		SDL_Quit();
		return 1;
	} catch (const LuaException& e) {
		std::string errname = "Lua error - " + e.file;
		std::string errmsg = e.file + ": " + e.what();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, errname.c_str(), errmsg.c_str(), NULL);
		SDL_Quit();
		return 1;
	}	

	SDL_Quit();
	return 0;
}