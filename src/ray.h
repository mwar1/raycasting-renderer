#pragma once

#include "vector.h"

#define NUM_RAYS 180
#define PI 3.14159

typedef struct Ray {
	Vec2 pos;
    Vec2 dir;
} Ray;

typedef struct Source {
	Vec2 pos;
	Ray rays[NUM_RAYS];
} Source;

void createSource(Source *src);