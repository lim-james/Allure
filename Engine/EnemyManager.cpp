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
	enemies.push_back(data);
}

void EnemyManager::SetSchedule(SpawnSchedule const & newSchedule) {
	schedule = newSchedule;
}

void EnemyManager::Awake() {
	EventsManager::Get()->Subscribe("PLAY", &EnemyManager::PlayHandler, this);
	EventsManager::Get()->Subscribe("BEAT", &EnemyManager::BeatHandler, this);
}

void EnemyManager::Start() {
	enabled = beat = false;
	beatCount = 0;
	index = 0;
}

void EnemyManager::Update() {
	if (!beat) return;
	beat = false;

	++beatCount;

	SpawnGroup& current = schedule[index];
	
	if (beatCount >= current.beatStride) {
		for (auto& pair : current.enemies) {
			EnemyData& enemy = enemies[pair.first];
			for (vec3f& position : pair.second) {
				CreateEnemy(enemy, position);
			}
		}
		++index;
		beatCount = 0;
	}
}

void EnemyManager::PlayHandler() {
	enabled = true;
}

void EnemyManager::BeatHandler() {
	beat = enabled;
}

void EnemyManager::CreateEnemy(EnemyData const & data, vec3f const& position) {
	Transform* const eTransform = data.prefab->Create();
	eTransform->SetLocalTranslation(position);

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
	if (data.weaponPrefab && combat && combat->IsActive()) {
		Transform* const wTransform = data.weaponPrefab->CreateIn(combat->weaponHolder);
		WeaponBase* const weapon = entities->GetComponent<WeaponBase>(wTransform->entity);
		weapon->audioPrefab = sfxPrefab;
		weapon->bulletMask = ENEMY;
		weapon->bulletLayer = ENEMY_BULLET;
		combat->SetWeapon(weapon);
		wTransform->SetLocalTranslation(weapon->HoldOffset());
	}
}
