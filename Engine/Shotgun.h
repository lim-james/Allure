#ifndef SHOTGUN_H
#define SHOTGUN_H

#include "Prefab.h"

struct Shotgun : Prefab {
	unsigned spriteSheet;
	Prefab* bulletPrefab;

	Shotgun();
	Transform* Create() override;
};

#endif
