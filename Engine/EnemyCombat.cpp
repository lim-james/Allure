#include "EnemyCombat.h"

#include <Events/EventsManager.h>

void EnemyCombat::SetWeapon(WeaponBase * const ptr) {
	weapon = ptr;
}

void EnemyCombat::Awake() {
	target = GetComponent<EnemyTarget>();
	//EventsManager::Get()->Subscribe("BEAT", &EnemyCombat::BeatHandler, this);
}

void EnemyCombat::Start() {
	bt = 0.f;
}

void EnemyCombat::Update() {
	//if (weapon && isTriggering) weapon->Hold(time->dt);

	const vec2f diff = target->GetTarget() - transform->GetWorldTranslation();
	weaponHolder->rotation.z = atanf(diff.y / fabs(diff.x)) * Math::toDeg;
}

void EnemyCombat::BeatHandler() {

}
