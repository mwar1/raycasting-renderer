#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    float x;
    float y;
} Vec2;

void normalise(Vec2 *vec);
Vec2 vectorFromAngle(float angle, bool normalise);
Vec2 addVectors(Vec2 vec1, Vec2 vec2);