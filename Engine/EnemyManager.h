#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "Script.h"
#include "Prefab.h"

#include "EnemyData.h"
#include "SpawnGroup.h"

struct EnemyManager : Script {

	using base_type = EnemyManager;

	vec2f boundary;
	Transform* player;
	Prefab* sfxPrefab;

	void AddEnemy(EnemyData const& data);

	void SetSchedule(SpawnSchedule const& newSchedule);

private:

	bool enabled, beat;

	std::vector<EnemyData> enemies;

	int index;
	unsigned beatCount;
	SpawnSchedule schedule;

	void Awake() override;
	void Start() override;
	void Update() override;

	void PlayHandler();
	void BeatHandler();

	void CreateEnemy(EnemyData const& data, vec3f const& position);

};

#endif
