#include "ray.h"

void createSource(Source *src) {
    float div = (2*PI) / NUM_RAYS;
    Vec2 pos = {src->pos.x, src->pos.y};

	for (int i=0; i<NUM_RAYS; i++) {
        Vec2 dir = vectorFromAngle(div * i, true);
        dir.x *= 100;
        dir.y *= 100;
		Ray new = {pos, addVectors(dir, pos)};
		src->rays[i] = new;
	}
}