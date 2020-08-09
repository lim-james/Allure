#ifndef PISTOL_PREVIEW_H
#define PISTOL_PREVIEW_H

#include "Prefab.h"

struct PistolPreview : Prefab {
	unsigned spriteSheet;
	PistolPreview();
	Transform* Create() override;
};

#endif
