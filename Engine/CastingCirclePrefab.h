#ifndef CASTING_CIRCLE_PREFAB_H
#define CASTING_CIRCLE_PREFAB_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct CastingCirclePrefab : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	CastingCirclePrefab();
	Transform* Create() override;
};

#endif
