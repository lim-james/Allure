#ifndef LASER_PREVIEW_H
#define LASER_PREVIEW_H

#include "Prefab.h"

struct LaserPreview : Prefab {
	unsigned spriteSheet;

	LaserPreview();
	Transform* Create() override;
};

#endif
