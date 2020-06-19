#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "Script.h"
#include "Prefab.h"

#include "EnemyData.h"

struct EnemyManager : Script {

	vec2f boundary;
	Transform* player;
	Prefab* sfxPrefab;

	void AddEnemy(EnemyData const& data);
	void AddWeapon(Prefab* const weapon);

private:

	struct EnemyGroup {
		EnemyData data;
		int beats;
	};

	bool enabled;

	std::vector<EnemyGroup> enemies;
	std::vector<Prefab*> weapons;

	void Awake() override;
	void Start() override;

	void KeyHandler(Events::Event* event);
	void BeatHandler();

};

#endif
