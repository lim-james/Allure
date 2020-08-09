#ifndef FIRE_ELEMENTAL_PREVIEW_H
#define FIRE_ELEMENTAL_PREVIEW_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct FireElementalPreview : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	FireElementalPreview();
	Transform* Create() override;
};

#endif
