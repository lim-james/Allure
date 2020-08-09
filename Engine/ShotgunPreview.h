#ifndef SHOTGUN_PREVIEW_H
#define SHOTGUN_PREVIEW_H

#include "Prefab.h"

struct ShotgunPreview : Prefab {
	unsigned spriteSheet;
	ShotgunPreview();
	Transform* Create() override;
};

#endif
