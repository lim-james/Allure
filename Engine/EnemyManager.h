#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "Script.h"
#include "Prefab.h"

#include "EnemyData.h"

struct EnemyManager : Script {

	using base_type = EnemyManager;

	vec2f boundary;
	Transform* player;
	Prefab* sfxPrefab;

	void AddEnemy(EnemyData const& data);

private:

	struct EnemyGroup {
		EnemyData data;
		int beats;
	};

	bool enabled, beat;

	std::vector<EnemyGroup> enemies;

	void Awake() override;
	void Start() override;
	void Update() override;

	void PlayHandler();
	void BeatHandler();

};

#endif
