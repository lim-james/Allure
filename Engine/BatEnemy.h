#ifndef BAT_ENEMY_H
#define BAT_ENEMY_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct BatEnemy : Prefab {

	unsigned spriteSheet;
	std::map<std::string, SpriteAnimationData> spriteData;

	BatEnemy();
	Transform* Create() override;
};

#endif
