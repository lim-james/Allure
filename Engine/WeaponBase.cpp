#include "WeaponBase.h"

WeaponBase::WeaponBase()
	: owner(nullptr)
	, target(nullptr) {}

void WeaponBase::SetOwner(Transform* const transform) {
	owner = transform;
}

void WeaponBase::SetTarget(Transform* const transform) {
	target = transform;
}

void WeaponBase::SetAudioSource(AudioSource * const source) {
	audio = source;
}
