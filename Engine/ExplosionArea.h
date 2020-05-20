#ifndef EXPLOSION_AREA_H
#define EXPLOSION_AREA_H

#include "Prefab.h"

struct ExplosionArea : Prefab {
	Transform* Create() override;
};

#endif
