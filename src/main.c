#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

const int SCREENWIDTH = 640;
const int SCREENHEIGHT = 480;

bool closeWindow = false;

int main(int argc, char* args[]) {	
	if(SDL_Init(SDL_INIT_VIDEO ) != 0) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}

	SDL_Window *window = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH, SCREENHEIGHT, 0);
	Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, renderFlags);

	while (!closeWindow) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			closeWindow = (event.type == SDL_QUIT);
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		SDL_RenderPresent(renderer);
	}
	
	// Clean up and quit
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
