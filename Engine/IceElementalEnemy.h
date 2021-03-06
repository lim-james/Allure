#ifndef ICE_ELEMENTAL_ENEMY_H
#define ICE_ELEMENTAL_ENEMY_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct IceElementalEnemy : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	IceElementalEnemy();
	Transform* Create() override;
};

#endif
