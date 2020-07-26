#ifndef ALTAR_CANDLE_PREFAB_H
#define ALTAR_CANDLE_PREFAB_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct AltarCandlePrefab : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	AltarCandlePrefab();
	Transform* Create() override;
};

#endif
