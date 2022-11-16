#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "ray.h"
#include "vector.h"

#define NUM_BOUNDS 8

const int SCREENWIDTH = 1280;
const int SCREENHEIGHT = 960;

float stripWidth = (float) SCREENWIDTH / NUM_RAYS;

bool closeWindow = false;

Boundary bounds[NUM_BOUNDS] = {{{5, 5}, {SCREENWIDTH-5, 5}},
					  		  {{5, 5}, {5, SCREENHEIGHT-5}},
					  	      {{5, SCREENHEIGHT-5}, {SCREENWIDTH-5, SCREENHEIGHT-5}},
					 		  {{SCREENWIDTH-5, 5}, {SCREENWIDTH-5, SCREENHEIGHT-5}},
							  {{200, 200}, {200, 300}},
							  {{300, 200}, {200, 200}},
							  {{300, 200}, {300, 300}},
							  {{300, 300}, {200, 300}}};

float mapRange(float input, float inMin, float inMax, float outMin, float outMax) {
	// Map the number 'input', in the range inMin-inMax to the range outMin-outMax
	if (input > inMax) return 0;

	return ( (((input - inMin) * (outMax - outMin)) / (inMax - inMin)) + outMin);
}

int main(int argc, char* args[]) {	
	if(SDL_Init(SDL_INIT_VIDEO ) != 0) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}

	int numDisplays = SDL_GetNumVideoDisplays();
	Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

	// Create the window for the top-down view
	SDL_Window *topDownWindow = SDL_CreateWindow("Raycasting Top Down", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH, SCREENHEIGHT, 0);
	SDL_Renderer *topDownRenderer = SDL_CreateRenderer(topDownWindow, -1, renderFlags);

	// Create the window for the 3D rendered view
	SDL_Window *window3D = SDL_CreateWindow("Raycasting 3D View", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREENWIDTH, SCREENHEIGHT, 0);
	SDL_Renderer *window3DRenderer = SDL_CreateRenderer(window3D, -1, renderFlags);

	if (numDisplays > 1) {
		SDL_Rect secondMonitor;
		SDL_GetDisplayBounds(1, &secondMonitor);
		SDL_SetWindowPosition(window3D, secondMonitor.x + 300, secondMonitor.y + 75);
	}

	Source lightSource;
	Vec2 srcPos = {SCREENWIDTH / 2, SCREENHEIGHT / 2};
	lightSource.pos = srcPos;
	lightSource.heading = -(PI / 4);
	lightSource.fov = 60;
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

		// Adjust the FOV
		if (keys[SDL_SCANCODE_Z] && lightSource.fov > 1) {
			lightSource.fov--;
			createSource(&lightSource);
		}
		if (keys[SDL_SCANCODE_X] && lightSource.fov < 179) {
			lightSource.fov++;
			createSource(&lightSource);
		}

		// Clear the screens
		SDL_SetRenderDrawColor(topDownRenderer, 0, 0, 0, 0);
		SDL_RenderClear(topDownRenderer);
		SDL_SetRenderDrawColor(window3DRenderer, 0, 0, 0, 0);
		SDL_RenderClear(window3DRenderer);

		//// Top Down Rendering
		// Draw the light rays
		SDL_SetRenderDrawColor(topDownRenderer, 255, 255, 255, 0);

		float rayDists[NUM_RAYS];
		Vec2 rayEnds[NUM_RAYS];
		for (int i=0; i<NUM_RAYS; i++) {
			float distance;
			Vec2 rayEnd = cast(&lightSource.rays[i], bounds, NUM_BOUNDS, &distance);
			rayDists[i] = distance;
			memcpy(&rayEnds[i], &rayEnd, sizeof(Vec2));
			if (rayEnd.x != -1 && rayEnd.y != -1) {
				SDL_RenderDrawLineF(topDownRenderer, 
								    lightSource.rays[i].pos.x, lightSource.rays[i].pos.y,
									rayEnd.x, rayEnd.y);
			}
		}
		// Draw the boundaries
		for (int i=0; i<NUM_BOUNDS; i++) {
			SDL_RenderDrawLineF(topDownRenderer, bounds[i].start.x, bounds[i].start.y,
								bounds[i].end.x, bounds[i].end.y);
		}

		Vec2 cameraPlaneDir;
		Vec2 cameraHeadingVec = vectorFromAngle(lightSource.heading, true);

		// Rotate the camera heading by 90 degrees to get the camera plane
		cameraPlaneDir.x = -cameraHeadingVec.y;
		cameraPlaneDir.y = cameraHeadingVec.x;

		//// 3D Rendering
		for (int i=0; i<NUM_RAYS; i++) {
			// Calculate the distance perpendicular to the camera plane
			// This removes the fisheye effect

			// Convert to variables similar to Wiki page
			// https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_two_points
			float x0 = rayEnds[i].x;
			float y0 = rayEnds[i].y;

			float x1 = lightSource.pos.x;
			float y1 = lightSource.pos.y;
			float x2 = lightSource.pos.x+cameraPlaneDir.x;
			float y2 = lightSource.pos.y+cameraPlaneDir.y;

			float perpDistance = fabs( (x2 - x1)*(y1 - y0) - (x1 - x0)*(y2 - y1) ) / magnitude((Vec2) {(x2 - x1), (y2 - y1)});

			float grayValue = mapRange(perpDistance, 0, SCREENHEIGHT, 255, 0);
			SDL_SetRenderDrawColor(window3DRenderer, grayValue, grayValue, grayValue, 255);

			float stripHeight = mapRange(perpDistance, 0, SCREENHEIGHT, SCREENHEIGHT, 0);

			SDL_FRect strip = {stripWidth * i, (SCREENHEIGHT/2) - (stripHeight/2), stripWidth, stripHeight};
			SDL_RenderFillRectF(window3DRenderer, &strip);
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
