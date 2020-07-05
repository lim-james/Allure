#ifndef BEAT_BULLET_H
#define BEAT_BULLET_H

#include "Prefab.h"

struct BeatBullet : Prefab {
	unsigned texture;

	BeatBullet();
	Transform* Create() override;
};

#endif
