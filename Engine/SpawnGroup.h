#ifndef SPAWN_GROUP_H
#define SPAWN_GROUP_H

#include <map>
#include <vector>
#include <Math/Vectors.h>

struct SpawnGroup {
	unsigned beatStride;
	std::map<unsigned, std::vector<vec3f>> enemies;
};

typedef std::vector<SpawnGroup> SpawnSchedule;

#endif
