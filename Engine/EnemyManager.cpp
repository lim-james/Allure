#include "EnemyManager.h"

#include "InputEvents.h"
#include "EnemyTarget.h"
#include "EnemyLife.h"
#include "EnemyCombat.h"

#include "Layers.h"

#include <Math/Random.hpp>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void EnemyManager::AddEnemy(EnemyData const & data) {
	enemies.push_back(EnemyGroup{ data, -data.beatDelay });
}

void EnemyManager::Awake() {
	EventsManager::Get()->Subscribe("PLAY", &EnemyManager::PlayHandler, this);
	EventsManager::Get()->Subscribe("BEAT", &EnemyManager::BeatHandler, this);
}

void EnemyManager::Start() {
	enabled = beat = false;
}

void EnemyManager::Update() {
	if (!beat) return;
	beat = false;

	for (EnemyGroup& group : enemies) {
		EnemyData& data = group.data;	

		++group.beats;
		if (data.beatStride > group.beats) continue;
		group.beats = 0;

		for (unsigned i = 0; i < group.data.batchSize; ++i) {
			Transform* const eTransform = group.data.prefab->Create();
			
			const float radius = Math::RandMinMax(30.f, 50.f);
			const float angle = Math::RandMinMax(0.f, 2.f * Math::PI);

			const vec3f displacement = vec3f(cos(angle) * radius, sin(angle) * radius, 0.f);
			eTransform->SetLocalTranslation(player->GetWorldTranslation() + displacement);

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
			if (data.weaponPrefab && combat) {
				Transform* const wTransform = data.weaponPrefab->CreateIn(combat->weaponHolder);
				WeaponBase* const weapon = entities->GetComponent<WeaponBase>(wTransform->entity);
				weapon->audioPrefab = sfxPrefab;
				weapon->bulletMask = ENEMY;
				weapon->bulletLayer = ENEMY_BULLET;
				combat->SetWeapon(weapon);
				wTransform->SetLocalTranslation(weapon->HoldOffset());
			}
		}
	}
}

void EnemyManager::PlayHandler() {
	enabled = true;
}

void EnemyManager::BeatHandler() {
	beat = enabled;
}
