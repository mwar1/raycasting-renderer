#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
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

	Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

	// Create the window for the top-down view
	SDL_Window *topDownWindow = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH, SCREENHEIGHT, 0);
	SDL_Renderer *topDownRenderer = SDL_CreateRenderer(topDownWindow, -1, renderFlags);

	// Create the window for the 3D rendered view
	SDL_Window *window3D = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH, SCREENHEIGHT, 0);
	SDL_Renderer *window3DRenderer = SDL_CreateRenderer(window3D, -1, renderFlags);

	Source lightSource;
	Vec2 srcPos = {SCREENWIDTH / 2, SCREENHEIGHT / 2};
	lightSource.pos = srcPos;
	lightSource.heading = (PI / 4);
	lightSource.fov = 90;
	createSource(&lightSource);

	while (!closeWindow) {
		SDL_Event event;
		Uint8 *keys = SDL_GetKeyboardState(NULL);
		while (SDL_PollEvent(&event)) {
			closeWindow = (event.type == SDL_QUIT || keys[SDL_SCANCODE_ESCAPE]);
		}

		// Move the light source depending on keyboard input
		if (keys[SDL_SCANCODE_W]) moveSource(&lightSource, 2.0f);
		if (keys[SDL_SCANCODE_S]) moveSource(&lightSource, -2.0f);
		if (keys[SDL_SCANCODE_A]) rotateSource(&lightSource, -(PI/180));
		if (keys[SDL_SCANCODE_D]) rotateSource(&lightSource, (PI/180));

		// Clear the screens
		SDL_SetRenderDrawColor(topDownRenderer, 0, 0, 0, 0);
		SDL_RenderClear(topDownRenderer);
		SDL_SetRenderDrawColor(window3DRenderer, 255, 0, 0, 0);
		SDL_RenderClear(window3DRenderer);

		// Draw the light rays
		SDL_SetRenderDrawColor(topDownRenderer, 255, 255, 255, 0);
		for (int i=0; i<NUM_RAYS; i++) {
			float distance;
			Vec2 rayEnd = cast(&lightSource.rays[i], bounds, NUM_BOUNDS, &distance);
			if (rayEnd.x != -1 && rayEnd.y != -1) {
				SDL_RenderDrawLineF(topDownRenderer, 
								    lightSource.rays[i].pos.x, lightSource.rays[i].pos.y,
									rayEnd.x, rayEnd.y);
			}
		}
		for (int i=0; i<NUM_BOUNDS; i++) {
			SDL_RenderDrawLineF(topDownRenderer, bounds[i].start.x, bounds[i].start.y,
								bounds[i].end.x, bounds[i].end.y);
		}

		SDL_RenderPresent(topDownRenderer);
		SDL_RenderPresent(window3DRenderer);
	}
	
	// Clean up and quit
	SDL_DestroyWindow(topDownWindow);
	SDL_DestroyWindow(window3D);
	SDL_DestroyRenderer(topDownRenderer);
	SDL_DestroyRenderer(window3DRenderer);
	SDL_Quit();
	return 0;
}
