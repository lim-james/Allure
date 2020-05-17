#ifndef WEAPON_BASE_H
#define WEAPON_BASE_H

#include "EntityManager.h"

#include "Transform.h"
#include "AudioSource.h"

#include <Encoding/Resource.h>

struct WeaponBase : Resource {

	WeaponBase();

	virtual void Initialize(EntityManager* const manager);
	virtual void SetPlayer(Transform* const transform);
	virtual void SetCrosshair(Transform* const transform);
	virtual void SetAudioSource(AudioSource* const source);

	virtual void Trigger(bool const& onBeat) = 0;
	virtual void Hold(float const& dt) = 0;
	virtual void Release(bool const& onBeat) = 0;

protected:

	EntityManager* entities;

	Transform* player;
	Transform* crosshair;
	AudioSource* audio;

};

#endif
