#ifndef GRENADE_LAUNCHER_H
#define GRENADE_LAUNCHER_H

#include "Prefab.h"

struct GrenadeLauncher : Prefab {
	unsigned spriteSheet;
	Prefab* bulletPrefab;

	GrenadeLauncher();
	Transform* Create() override;
};

#endif
