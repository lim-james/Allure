#ifndef CANDLE_1_PREFAB_H
#define CANDLE_1_PREFAB_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct Candle1Prefab : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	Candle1Prefab();
	Transform* Create() override;
};

#endif
