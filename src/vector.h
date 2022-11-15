#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct Vec2 {
    float x;
    float y;
} Vec2;

void normalise(Vec2 *vec);
Vec2 vectorFromAngle(float angle, bool normalise);
Vec2 addVectors(Vec2 vec1, Vec2 vec2);
float magnitude(Vec2 vec);

#endif
