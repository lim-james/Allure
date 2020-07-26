#ifndef BASIC_ENEMY_H
#define BASIC_ENEMY_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct BasicEnemy : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	BasicEnemy();
	Transform* Create() override;
};

#endif
