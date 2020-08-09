#ifndef AUTOMATIC_PREVIEW_H
#define AUTOMATIC_PREVIEW_H

#include "Prefab.h"

struct AutomaticPreview : Prefab {
	unsigned spriteSheet;
	AutomaticPreview();
	Transform* Create() override;
};

#endif
