#include "EnemyCombat.h"

#include <Events/EventsManager.h>

void EnemyCombat::SetWeapon(WeaponBase * const ptr) {
	weapon = ptr;
	weapon->SetOwner(transform);
	weapon->SetTarget(target->player);
}

void EnemyCombat::Awake() {
	life = GetComponent<EnemyLife>();
	target = GetComponent<EnemyTarget>();
	EventsManager::Get()->Subscribe("BEAT", &EnemyCombat::BeatHandler, this);
}

void EnemyCombat::Start() {
	holdDuration = 0.1f;
	bt = 0.f;
}

void EnemyCombat::Update() {
	if (life->IsStunned() || life->IsDead()) return;

	if (weapon) {
		if (isBeat) {
			weapon->Trigger();
			isBeat = false;
		}

		//if (bt > 0.f) {
		//	bt -= time->dt;
		//	if (bt <= 0.f) {
		//		weapon->Release();
		//	} else {
		//		weapon->Hold(time->dt);
		//	}
		//} 
	}	

	const vec2f diff = target->GetTarget() - transform->GetWorldTranslation();
	weaponHolder->SetLocalRotation(vec3f(0.f, 0.f, atanf(diff.y / fabs(diff.x)) * Math::toDeg));
}

void EnemyCombat::BeatHandler() {
	isBeat = life->IsStunned() || life->IsDead();
}
