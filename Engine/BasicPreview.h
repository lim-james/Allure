#ifndef BASIC_PREVIEW_H
#define BASIC_PREVIEW_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct BasicPreview : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	BasicPreview();
	Transform* Create() override;
};

#endif
