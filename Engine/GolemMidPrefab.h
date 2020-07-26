#ifndef GOLEM_MID_PREFAB_H
#define GOLEM_MID_PREFAB_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct GolemMidPrefab : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	GolemMidPrefab();
	Transform* Create() override;
};

#endif
