#ifndef ENEMY_DATA_H
#define ENEMY_DATA_H

#include "Prefab.h"

#define LOW_RISK	0
#define MEDIUM_RISK 1
#define HIGH_RISK	2

struct EnemyData {
	Prefab* prefab;
	short risk;
	unsigned points;
	unsigned spawnLevel;
};

#endif
