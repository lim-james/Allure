#ifndef PISTOL_H
#define PISTOL_H

#include "Prefab.h"

struct Pistol : Prefab {
	unsigned spriteSheet;
	Prefab* bulletPrefab;

	Pistol();
	Transform* Create() override;
};

#endif
