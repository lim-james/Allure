#ifndef AUTOMATIC_H
#define AUTOMATIC_H

#include "Prefab.h"

struct Automatic : Prefab {
	unsigned spriteSheet;
	Prefab* bulletPrefab;

	Automatic();
	Transform* Create() override;
};

#endif
