#ifndef GOLEM_LITTLE_PREFAB_H
#define GOLEM_LITTLE_PREFAB_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct GolemLittlePrefab : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	GolemLittlePrefab();
	Transform* Create() override;
};

#endif