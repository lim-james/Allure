#include "EnemyManager.h"

#include "EnemyTarget.h"
#include "EnemyLife.h"

#include <Math/Random.hpp>

void EnemyManager::AddEnemy(EnemyData const & data) {
	enemies.push_back(data);
}

void EnemyManager::Awake() {
	bt = 0.f;
	spawnDelay = 1.f;
}

void EnemyManager::Update() {
	if (bt > spawnDelay) {
		bt = 0.f;
		for (EnemyData const& data : enemies) {
			Transform* const eTransform = data.prefab->Create();
			eTransform->translation.x = Math::RandMinMax(-boundary.x, boundary.x);
			eTransform->translation.y = Math::RandMinMax(-boundary.y, boundary.y);

			EnemyTarget* const target = entities->GetComponent<EnemyTarget>(eTransform->entity);
			target->player = player;
	
			EnemyLife* const life = entities->GetComponent<EnemyLife>(eTransform->entity);
			life->points = data.points;
		}
	} else {
		bt += time->dt;
	}
}

