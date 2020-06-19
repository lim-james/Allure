#include "EnemyManager.h"

#include "InputEvents.h"
#include "EnemyTarget.h"
#include "EnemyLife.h"
#include "EnemyCombat.h"

#include "SniperScript.h"
#include "PistolScript.h"
#include "AutomaticScript.h"

#include <Math/Random.hpp>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void EnemyManager::AddEnemy(EnemyData const & data) {
	enemies.push_back(EnemyGroup{ data, -data.beatDelay });
}

void EnemyManager::AddWeapon(Prefab * const weapon) {
	weapons.push_back(weapon);
}

void EnemyManager::Awake() {
	EventsManager::Get()->Subscribe("KEY_INPUT", &EnemyManager::KeyHandler, this);
	EventsManager::Get()->Subscribe("BEAT", &EnemyManager::BeatHandler, this);
}

void EnemyManager::Start() {
	enabled = false;
}

void EnemyManager::KeyHandler(Events::Event* event) {
	auto input = static_cast<Events::KeyInput*>(event);

	if (input->key == GLFW_KEY_SPACE && input->action == GLFW_PRESS) {
		enabled = !enabled;
	}
}

void EnemyManager::BeatHandler() {
	if (!enabled) return;

	for (EnemyGroup& group : enemies) {
		EnemyData& data = group.data;	

		++group.beats;
		if (data.beatStride > group.beats) continue;
		group.beats = 0;

		for (unsigned i = 0; i < group.data.batchSize; ++i) {
			Transform* const eTransform = group.data.prefab->Create();
			eTransform->translation.x = Math::RandMinMax(-boundary.x, boundary.x);
			eTransform->translation.y = Math::RandMinMax(-boundary.y, boundary.y);

			SpriteRender* const render = entities->GetComponent<SpriteRender>(eTransform->entity);
			render->tint = data.colour;

			EnemyLife* const life = entities->GetComponent<EnemyLife>(eTransform->entity);
			life->shield = data.shield;
			life->health = data.health;
			life->points = data.points;
			life->bodyDamage = data.bodyDamage;

			EnemyTarget* const target = entities->GetComponent<EnemyTarget>(eTransform->entity);
			target->player = player;
			target->boundary = boundary;
			target->farStyle = data.farStyle;
			target->nearStyle = data.nearStyle;
			target->style = &target->farStyle;

			EnemyCombat* const combat = entities->GetComponent<EnemyCombat>(eTransform->entity);
			if (combat) {
				Transform* const wTransform = weapons[0]->CreateIn(combat->weaponHolder);
				WeaponBase* const weapon = entities->GetComponent<SniperScript>(wTransform->entity);
				weapon->audioPrefab = sfxPrefab;
				combat->SetWeapon(weapon);
				wTransform->translation = weapon->HoldOffset();
			}
		}
	}
}
