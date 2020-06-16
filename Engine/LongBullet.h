#ifndef LONG_BULLET_H
#define LONG_BULLET_H

#include "Prefab.h"

struct LongBullet : Prefab {
	unsigned spriteSheet;

	LongBullet();
	Transform* Create() override;
};

#endif
