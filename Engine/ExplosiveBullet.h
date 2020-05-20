#ifndef EXPLOSIVE_BULLET_H
#define EXPLOSIVE_BULLET_H

#include "Prefab.h"

struct ExplosiveBullet : Prefab {
	unsigned spriteSheet;

	Prefab* explosionPrefab;
	ExplosiveBullet();
	Transform* Create() override;
};

#endif
