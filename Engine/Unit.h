#ifndef UNIT_H
#define UNIT_H

#include "Transform.h"

#include <vector>
#include <map>
#include <stack>

struct Unit {

	Transform * transform;
	float viewRange;
	float AOE;
	bool canAttack;
	bool chasing;

	std::vector<vec2i> path;
	std::vector<int> vision;
	std::map<unsigned, Unit*> visibleUnits;

	std::vector<bool> visited;
	std::stack<vec2i> queuedPositions;

};

#endif
