#ifndef LASER_SCRIPT_H
#define LASER_SCRIPT_H

#include "WeaponBase.h"
#include "Prefab.h"

#include "SpriteRender.h"
#include "Collider.h"

struct LaserScript : WeaponBase {

	Prefab* standardPrefab;
	Prefab* explosivePrefab;

	void Trigger() override;
	void Hold(float const& dt) override;
	void Release() override;
	vec3f HoldOffset() const override;

private:

	Transform* laserTransform;
	SpriteRender* laserRender;
	Collider* laserCollider;

	void Start() override;
	void SetLaserState(bool const& state);

};

#endif
