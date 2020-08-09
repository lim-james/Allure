#ifndef CYCLOPS_PREVIEW_H
#define CYCLOPS_PREVIEW_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct CyclopsPreview : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	CyclopsPreview();
	Transform* Create() override;
};

#endif
