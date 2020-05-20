#ifndef BASIC_BULLET_H
#define BASIC_BULLET_H

#include "Prefab.h"

struct BasicBullet : Prefab {
	unsigned spriteSheet;

	BasicBullet();
	Transform* Create() override;
};

#endif
