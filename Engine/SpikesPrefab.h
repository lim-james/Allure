#ifndef SPIKES_PREFAB_H
#define SPIKES_PREFAB_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct SpikesPrefab : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	SpikesPrefab();
	Transform* Create() override;
};

#endif
