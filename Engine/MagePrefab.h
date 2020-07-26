#ifndef MAGE_PREFAB_H
#define MAGE_PREFAB_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct MagePrefab : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	MagePrefab();
	Transform* Create() override;
};

#endif
