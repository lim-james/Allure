#ifndef SKELETON_ARCHER_PREFAB_H
#define SKELETON_ARCHER_PREFAB_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct SkeletonArcherPrefab : Prefab {
	unsigned spriteSheet;
	SADMap spriteData;

	SkeletonArcherPrefab();
	Transform* Create() override;
};

#endif
