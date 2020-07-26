#ifndef CHEST_PREFAB_H
#define CHEST_PREFAB_H

#include "Prefab.h"

struct ChestPrefab : Prefab {
	unsigned spriteSheet;

	ChestPrefab();
	Transform* Create() override;
};

#endif
