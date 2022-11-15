#include <string.h>
#include "ray.h"

void createSource(Source *src) {
    float div = (2*PI) / NUM_RAYS;
    Vec2 pos = {src->pos.x, src->pos.y};

	for (int i=0; i<NUM_RAYS; i++) {
        Vec2 dir = vectorFromAngle(div * i, true);
		Ray new = {pos, addVectors(dir, pos)};
		src->rays[i] = new;
	}
}

Vec2 cast(Ray *ray, Boundary bounds[], int numBounds) {
    float closestDist = INFINITY;
    Vec2 closestPoint = {-1, -1};

    for (int i=0; i<numBounds; i++) {
        Vec2 pt = collisionPoint(&bounds[i], ray);
        if (pt.x != -1 && pt.y != -1) {
            float mag = magnitude((Vec2) {pt.x - ray->pos.x, pt.y - ray->pos.y});
            if (mag < closestDist) {
                closestDist = mag;
                memcpy(&closestPoint, &pt, sizeof(Vec2));
            }
        }
    }
    return closestPoint;
}

Vec2 collisionPoint(Boundary *bound, Ray *ray) {
    // Convert to variables similar to Wiki page
    // https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line_segment
    float x1 = bound->start.x;
    float x2 = bound->end.x;
    float x3 = ray->pos.x;
    float x4 = ray->end.x;

    float y1 = bound->start.y;
    float y2 = bound->end.y;
    float y3 = ray->pos.y;
    float y4 = ray->end.y;

    Vec2 result = {-1, -1};

    float denominator = ((x1 - x2)*(y3 - y4)) - ((y1 - y2)*(x3 - x4));
    if (denominator == 0) {
        // Lines are parallel, so will never meet
        return result;
    }

    float t = ((x1 - x3)*(y3 - y4) - (y1 - y3)*(x3 - x4)) / denominator;
    float u = ((x1 - x3)*(y1 - y2) - (y1 - y3)*(x1 - x2)) / denominator;

    if (t >= 0 && t <= 1 && u >= 0) {
        // The lines do collide, so calculate the point of collision
        result.x = x1 + t*(x2 - x1);
        result.y = y1 + t*(y2 - y1);
    }
    return result;
}

void moveSource(Source *src, Vec2 newPos) {
    memcpy(&src->pos, &newPos, sizeof(Vec2));
    createSource(src);
}
