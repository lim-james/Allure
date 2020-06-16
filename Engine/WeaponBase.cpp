#include "WeaponBase.h"

WeaponBase::WeaponBase()
	: player(nullptr)
	, crosshair(nullptr) {}

void WeaponBase::SetPlayer(Transform* const transform) {
	player = transform;
}

void WeaponBase::SetCrosshair(Transform* const transform) {
	crosshair = transform;
}

void WeaponBase::SetAudioSource(AudioSource * const source) {
	audio = source;
}
