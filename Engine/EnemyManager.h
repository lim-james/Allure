#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "Script.h"
#include "Prefab.h"

#include "EnemyData.h"

struct EnemyManager : Script {

	vec2f boundary;
	Transform* player;

	void AddEnemy(EnemyData const& data);

private:

	bool enabled;

	float bt;
	float spawnDelay;

	std::vector<EnemyData> enemies;

	void Awake() override;
	void Start() override;
	void Update() override;

	void KeyHandler(Events::Event* event);

};

#endif
