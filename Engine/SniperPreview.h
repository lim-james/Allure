#ifndef SNIPER_PREVIEW_H
#define SNIPER_PREVIEW_H

#include "Prefab.h"

struct SniperPreview : Prefab {
	unsigned spriteSheet;
	SniperPreview();
	Transform* Create() override;
};

#endif
