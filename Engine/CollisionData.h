#ifndef COLLISION_DATA_H
#define COLLISION_DATA_H

#include <Math/Vectors.h>
#include "Collider.h"

struct CollisionData {
	Collider* target;
	float distance;
	vec3f position;
	vec3f normal;

	CollisionData();
};

#endif
