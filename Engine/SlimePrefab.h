#ifndef SLIME_PREFAB_H
#define SLIME_PREFAB_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct SlimePrefab : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	SlimePrefab();
	Transform* Create() override;
};

#endif
