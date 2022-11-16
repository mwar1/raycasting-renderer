#ifndef RAY_H
#define RAY_H

#include <SDL2/SDL.h>
#include "vector.h"

#define NUM_RAYS 500
#define PROJ_PLANE_WIDTH 1000
#define PROJ_PLANE_DIV PROJ_PLANE_WIDTH / NUM_RAYS
#define PI 3.14159

typedef struct Ray {
	Vec2 pos;
    Vec2 end;
} Ray;

typedef struct Source {
	Vec2 pos;
    float heading;
	Ray rays[NUM_RAYS];
    float fov;
} Source;

typedef struct Boundary {
    Vec2 start;
    Vec2 end;
    SDL_Color colour;
} Boundary;

void createSource(Source *src);
Vec2 cast(struct Ray *ray, Boundary bounds[], int numBounds, float *closestDistance, int *boundIndex);
Vec2 collisionPoint(struct Boundary *bound, Ray *ray);
void moveSource(Source *src, float amount);
void rotateSource(Source *src, float angle);

#endif
