#ifndef WEAPON_BASE_H
#define WEAPON_BASE_H

#include "Script.h"

#include "Prefab.h"
#include "Transform.h"
#include "AudioSource.h"

struct WeaponBase : Script {

	Prefab* bulletPrefab;

	WeaponBase();

	void SetOwner(Transform* const transform);
	void SetTarget(Transform* const transform);
	void SetAudioSource(AudioSource* const source);

	virtual void Trigger() = 0;
	virtual void Hold(float const& dt) = 0;
	virtual void Release() = 0;
	virtual vec3f HoldOffset() const = 0;

protected:

	Transform* owner;
	Transform* target;
	AudioSource* audio;

};

#endif
