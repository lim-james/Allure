#ifndef BAT_PREVIEW_H
#define BAT_PREVIEW_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct BatPreview : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	BatPreview();
	Transform* Create() override;
};

#endif
