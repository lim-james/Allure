#ifndef WEAPON_BASE_H
#define WEAPON_BASE_H

#include "EntityManager.h"

#include "Transform.h"

class WeaponBase {

public:

	WeaponBase();

	void Initialize(EntityManager* const manager);
	void SetPlayer(Transform* const transform);
	void SetCrosshair(Transform* const transform);

	virtual void Trigger() = 0;
	virtual void Hold(float const& dt) = 0;
	virtual void Release() = 0;

protected:

	EntityManager* entities;

	Transform* player;
	Transform* crosshair;

};

#endif
