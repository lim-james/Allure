#ifndef ENEMY_DATA_H
#define ENEMY_DATA_H

#include "Prefab.h"
#include <Math/Vectors.h>

#define TARGET_LOCKON	0
#define TARGET_DASH		1
#define TARGET_RANDOM	2

#define MOVEMENT_CONSTANT	0
#define MOVEMENT_DASH		1
#define MOVEMENT_DISTANT	3
#define MOVEMENT_TELEPORT	4

struct TargetStyle {
	short type;
	short movement;
	float speed;
	float radius;
};

struct EnemyData {
	Prefab* prefab;
	Prefab* weaponPrefab;

	vec4f colour;

	// life

	int shield;
	int health;
	unsigned points;
	bool bodyDamage;

	//target 

	TargetStyle farStyle;
	TargetStyle nearStyle;

	// spawn

	bool repeat;
	int beatDelay;
	int beatStride;
	unsigned batchSize;

};

#endif
