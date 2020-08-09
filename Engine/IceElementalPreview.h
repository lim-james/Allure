#ifndef ICE_ELEMENTAL_PREVIEW_H
#define ICE_ELEMENTAL_PREVIEW_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct IceElementalPreview : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	IceElementalPreview();
	Transform* Create() override;
};

#endif
