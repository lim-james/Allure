#ifndef COLLISION_DATA_H
#define COLLISION_DATA_H

#include <Math/Vectors.h>
#include "BoxCollider.h"

struct CollisionData {
	BoxCollider* target;
	float distance;
	vec3f position;
	vec3f normal;
};

#endif
