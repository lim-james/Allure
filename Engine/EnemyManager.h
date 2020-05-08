#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "Script.h"

struct EnemyManager : Script {

	Transform* player;

private:

	float bt;
	float spawnDelay;

	void Awake() override;
	void Update() override;

	void CreateEnemy();

};

#endif
