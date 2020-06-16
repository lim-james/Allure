#ifndef SNIPER_H
#define SNIPER_H

#include "Prefab.h"

struct Sniper : Prefab {
	unsigned spriteSheet;
	Prefab* bulletPrefab;

	Sniper();
	Transform* Create() override;
};

#endif
