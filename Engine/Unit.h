#ifndef UNIT_H
#define UNIT_H

#include "Transform.h"

#include <vector>

struct Unit {

	Transform * transform;
	float viewRange;
	float AOE;
	bool canAttack;

	std::vector<vec2i> path;
	std::vector<int> vision;

};

#endif
