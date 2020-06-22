#ifndef LASER_SCRIPT_H
#define LASER_SCRIPT_H

#include "WeaponBase.h"

#include "SpriteRender.h"
#include "Collider.h"

struct LaserScript : WeaponBase {

	using base_type = WeaponBase;

	void Trigger() override;
	void Hold(float const& dt) override;
	void Release() override;
	vec3f HoldOffset() const override;
	std::string Name() const override;

private:

	Transform* laserTransform;
	SpriteRender* laserRender;
	Collider* laserCollider;

	void Start() override;
	void SetLaserState(bool const& state);

};

#endif
