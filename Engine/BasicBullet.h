#ifndef BASIC_BULLET_H
#define BASIC_BULLET_H

#include "Prefab.h"

struct BasicBullet : Prefab {
	Transform* Create() override;
};

#endif
