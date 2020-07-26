#ifndef CANDLE_2_PREFAB_H
#define CANDLE_2_PREFAB_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct Candle2Prefab : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	Candle2Prefab();
	Transform* Create() override;
};

#endif
