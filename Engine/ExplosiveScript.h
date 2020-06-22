#ifndef EXPLOSIVE_SCRIPT_H
#define EXPLOSIVE_SCRIPT_H

#include "Script.h"
#include "Prefab.h"

struct ExplosiveScript : Script {

	using base_type = ExplosiveScript;

	Prefab* explosionPrefab;
	void OnDestroy() override;

};

#endif
