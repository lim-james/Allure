#ifndef CON_PREFAB_H
#define CON_PREFAB_H

#include "Prefab.h"
#include "SpriteAnimationData.h"

struct CoinPrefab : Prefab {
	unsigned coinSheet, pickupSheet;
	SADMap coinData, pickupData;

	CoinPrefab();
	Transform* Create() override;
};

#endif
