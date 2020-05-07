#include "WeaponBase.h"

WeaponBase::WeaponBase()
	: entities(nullptr)
	, player(nullptr)
	, crosshair(nullptr) {}

void WeaponBase::Initialize(EntityManager * const manager) {
	entities = manager;
}

void WeaponBase::SetPlayer(Transform* const transform) {
	player = transform;
}

void WeaponBase::SetCrosshair(Transform* const transform) {
	crosshair = transform;
}

void WeaponBase::SetAudioSource(AudioSource * const source) {
	audio = source;
}
