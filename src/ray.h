#ifndef RAY_H
#define RAY_H

#include "vector.h"

#define NUM_RAYS 180
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
} Boundary;

void createSource(Source *src);
Vec2 cast(struct Ray *ray, Boundary bounds[], int numBounds, float *closestDistance);
Vec2 collisionPoint(struct Boundary *bound, Ray *ray);
void moveSource(Source *src, float amount);
void rotateSource(Source *src, float angle);

#endif
