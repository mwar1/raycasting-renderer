#include "vector.h"

void normalise(Vec2 *vec) {
    float length = sqrt((vec->x * vec->x) + (vec->y * vec->y));
    if (length != 0) {
        vec->x /= length;
        vec->y /= length;
    }
}

Vec2 vectorFromAngle(float angle, bool normal) {
    // Returns a vector pointing in the direction given by angle (in radians)
    // The vector is normalised if normal is set to true
    Vec2 vec;
    vec.x = cos(angle);
    vec.y = sin(angle);
    if (normal) normalise(&vec);

    return vec;
}

Vec2 addVectors(Vec2 vec1, Vec2 vec2) {
    Vec2 result;
    result.x = vec1.x + vec2.x;
    result.y = vec1.y + vec2.y;
    return result;
}

float magnitude(Vec2 vec) {
    return sqrt((vec.x * vec.x) + (vec.y * vec.y));
}
