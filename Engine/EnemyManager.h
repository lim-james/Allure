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

	struct EnemyGroup {
		EnemyData data;
		int beats;
	};

	bool enabled;

	std::vector<EnemyGroup> enemies;

	void Awake() override;
	void Start() override;

	void KeyHandler(Events::Event* event);
	void BeatHandler();

};

#endif
