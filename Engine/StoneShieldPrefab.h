#ifndef STONE_SHIELD_PREFAB_H
#define STONE_SHIELD_PREFAB_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct StoneShieldPrefab : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	StoneShieldPrefab();
	Transform* Create() override;
};

#endif
