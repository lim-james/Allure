#include "WeaponBase.h"

#include "Layers.h"

WeaponBase::WeaponBase()
	: owner(nullptr)
	, target(nullptr) {}

void WeaponBase::SetOwner(Transform* const transform) {
	owner = transform;
	if (owner) {
		isPlayer = entities->GetLayer(owner->entity) == PLAYER;
		collider->SetActive(false);
	} else {
		isPlayer = false;
		collider->SetActive(true);
	}
}

void WeaponBase::SetTarget(Transform* const transform) {
	target = transform;
}

void WeaponBase::Awake() {
	collider = GetComponent<SphereCollider>();
}


