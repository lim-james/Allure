#ifndef EXPLOSIVE_BULLET_H
#define EXPLOSIVE_BULLET_H

#include "Prefab.h"

struct ExplosiveBullet : Prefab {
	Transform* Create() override;
};

#endif
