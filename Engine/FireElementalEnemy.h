#ifndef FIRE_ELEMENTAL_ENEMY_H
#define FIRE_ELEMENTAL_ENEMY_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct FireElementalEnemy : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	FireElementalEnemy();
	Transform* Create() override;
};

#endif
