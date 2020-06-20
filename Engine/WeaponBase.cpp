#include "WeaponBase.h"

#include "Layers.h"

WeaponBase::WeaponBase()
	: owner(nullptr)
	, target(nullptr) {}

void WeaponBase::SetOwner(Transform* const transform) {
	owner = transform;
	isPlayer = entities->GetLayer(transform->entity) == PLAYER;
}

void WeaponBase::SetTarget(Transform* const transform) {
	target = transform;
}
