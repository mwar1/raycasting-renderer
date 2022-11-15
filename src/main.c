#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "ray.h"
#include "vector.h"

#define NUM_BOUNDS 7

const int SCREENWIDTH = 1280;
const int SCREENHEIGHT = 960;

bool closeWindow = false;

Boundary bounds[NUM_BOUNDS] = {{{5, 5}, {SCREENWIDTH-5, 5}},
					  		  {{5, 5}, {5, SCREENHEIGHT-5}},
					  	      {{5, SCREENHEIGHT-5}, {SCREENWIDTH-5, SCREENHEIGHT-5}},
					 		  {{SCREENWIDTH-5, 5}, {SCREENWIDTH-5, SCREENHEIGHT-5}},
							  {{100, 200}, {500, 800}},
							  {{1000, 60}, {1200, 900}},
							  {{200, 500}, {700, 450}}};

int main(int argc, char* args[]) {	
	if(SDL_Init(SDL_INIT_VIDEO ) != 0) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}

	SDL_Window *window = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH, SCREENHEIGHT, 0);
	Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, renderFlags);

	Source lightSource;
	Vec2 srcPos = {SCREENWIDTH / 2, SCREENHEIGHT / 2};
	lightSource.pos = srcPos;
	createSource(&lightSource);

	while (!closeWindow) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			closeWindow = (event.type == SDL_QUIT);
		}

		// Clear the screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		// Draw the light rays
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		for (int i=0; i<NUM_RAYS; i++) {
			Vec2 rayEnd = cast(&lightSource.rays[i], bounds, NUM_BOUNDS);
			//printf("ray end: %f, %f\n", rayEnd.x, rayEnd.y);
			if (rayEnd.x != -1 && rayEnd.y != -1) {
				SDL_RenderDrawLineF(renderer, 
								    lightSource.rays[i].pos.x, lightSource.rays[i].pos.y,
									rayEnd.x, rayEnd.y);
			}
		}
		for (int i=0; i<NUM_BOUNDS; i++) {
			SDL_RenderDrawLineF(renderer, bounds[i].start.x, bounds[i].start.y,
								bounds[i].end.x, bounds[i].end.y);
		}

		SDL_RenderPresent(renderer);
	}
	
	// Clean up and quit
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
