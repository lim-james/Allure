#ifndef CYCLOPS_ENEMY_H
#define CYCLOPS_ENEMY_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct CyclopsEnemy : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	CyclopsEnemy();
	Transform* Create() override;
};

#endif
