#ifndef SPAWNER_PREFAB_H
#define SPAWNER_PREFAB_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct SpawnerPrefab : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	SpawnerPrefab();
	Transform* Create() override;
};

#endif
