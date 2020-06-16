#ifndef LASER_H
#define LASER_H

#include "Prefab.h"

struct Laser : Prefab {
	unsigned spriteSheet;
	Prefab* bulletPrefab;

	Laser();
	Transform* Create() override;
};

#endif
