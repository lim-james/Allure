#ifndef GRENADE_LAUNCHER_PREVIEW_H
#define GRENADE_LAUNCHER_PREVIEW_H

#include "Prefab.h"

struct GrenadeLauncherPreview : Prefab {
	unsigned spriteSheet;
	GrenadeLauncherPreview();
	Transform* Create() override;
};

#endif
