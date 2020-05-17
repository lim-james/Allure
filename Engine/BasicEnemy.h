#ifndef BASIC_ENEMY_H
#define BASIC_ENEMY_H

#include "Prefab.h"

struct BasicEnemy : Prefab {
	Transform* Create() override;
};

#endif
