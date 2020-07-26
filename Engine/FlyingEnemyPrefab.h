#ifndef FLYING_ENEMY_PREFAB_H
#define FLYING_ENEMY_PREFAB_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct FlyingEnemyPrefab : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	FlyingEnemyPrefab();
	Transform* Create() override;
};

#endif
