#ifndef BEAT_BULLET_H
#define BEAT_BULLET_H

#include "Prefab.h"

#include "Material.h"

struct BeatBullet : Prefab {
	unsigned texture;
	Material::Base* material;

	BeatBullet();
	Transform* Create() override;
};

#endif
