#ifndef WEAPON_BASE_H
#define WEAPON_BASE_H

#include "Script.h"

#include "Prefab.h"
#include "Transform.h"
#include "AudioSource.h"
#include "SphereCollider.h"

#include <Bit/BitField.h>

struct WeaponBase : Script {

	using base_type = WeaponBase;

	BitField bulletMask;
	unsigned bulletLayer;

	Prefab* bulletPrefab;
	Prefab* audioPrefab;

	WeaponBase();

	void SetOwner(Transform* const transform);
	void SetTarget(Transform* const transform);

	virtual void Trigger() = 0;
	virtual void Hold(float const& dt) = 0;
	virtual void Release() = 0;
	virtual vec3f HoldOffset() const = 0;
	virtual std::string Name() const = 0;

protected:

	Transform* owner;
	Transform* target;
	SphereCollider* collider;

	bool isPlayer;

	void Awake() override;
	float GetForce() const;

};

#endif
