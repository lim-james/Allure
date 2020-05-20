#ifndef EXPLOSIVE_BULLET_H
#define EXPLOSIVE_BULLET_H

#include "Prefab.h"

struct ExplosiveBullet : Prefab {
	Prefab* explosionPrefab;
	Transform* Create() override;
};

#endif
