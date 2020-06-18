#ifndef EXPLOSIVE_SCRIPT_H
#define EXPLOSIVE_SCRIPT_H

#include "Script.h"
#include "Prefab.h"

struct ExplosiveScript : Script {

	Prefab* explosionPrefab;
	void OnDestroy() override;

};

#endif
