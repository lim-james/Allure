#include "EnemyManager.h"

#include "EnemyTarget.h"
#include "EnemyLife.h"
#include "InputEvents.h"

#include <Math/Random.hpp>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void EnemyManager::AddEnemy(EnemyData const & data) {
	enemies.push_back(data);
}

void EnemyManager::Awake() {
	spawnDelay = 1.f;

	EventsManager::Get()->Subscribe("KEY_INPUT", &EnemyManager::KeyHandler, this);
}

void EnemyManager::Start() {
	bt = 0.f;
	enabled = false;
}

void EnemyManager::Update() {
	if (!enabled) return;
	
	if (bt > spawnDelay) {
		bt = 0.f;
		for (EnemyData const& data : enemies) {
			Transform* const eTransform = data.prefab->Create();
			eTransform->translation.x = Math::RandMinMax(-boundary.x, boundary.x);
			eTransform->translation.y = Math::RandMinMax(-boundary.y, boundary.y);

			EnemyTarget* const target = entities->GetComponent<EnemyTarget>(eTransform->entity);
			target->player = player;
	
			EnemyLife* const life = entities->GetComponent<EnemyLife>(eTransform->entity);
			life->lives = data.lives;
			life->points = data.points;
		}
	} else {
		bt += time->dt;
	}
}

void EnemyManager::KeyHandler(Events::Event* event) {
	auto input = static_cast<Events::KeyInput*>(event);

	if (input->key == GLFW_KEY_SPACE && input->action == GLFW_PRESS) {
		enabled = !enabled;
	}
}
