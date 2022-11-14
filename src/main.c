#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "vector.h"

#define NUM_RAYS 180
#define PI 3.14159

const int SCREENWIDTH = 640;
const int SCREENHEIGHT = 480;

bool closeWindow = false;

typedef struct Ray {
	Vec2 pos;
	float angle;
} Ray;

typedef struct Source {
	Vec2 pos;
	Ray rays[NUM_RAYS];
} Source;

void createSource(Source *src) {
	for (int i=0; i<NUM_RAYS; i++) {
		Ray new = {{src->pos.x, src->pos.y}, (2*PI/NUM_RAYS) * i};
		src->rays[i] = new;
	}
}

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
		for (int i=0; i<NUM_RAYS; i++) {
			// SDL_RenderDrawLineF(renderer, )
		}

		SDL_RenderPresent(renderer);
	}
	
	// Clean up and quit
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
