#ifndef UNIT_H
#define UNIT_H

#include "Transform.h"

#include <vector>

struct Unit {

	Transform * transform;
	float range;

	std::vector<vec2i> path;

};

#endif
