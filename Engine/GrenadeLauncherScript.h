#ifndef GRENANDE_LAUNCH_H
#define GRENANDE_LAUNCH_H

#include "WeaponBase.h"

struct GrenadeLauncherScript : WeaponBase {

	using base_type = WeaponBase;

	void Trigger() override;
	void Hold(float const& dt) override;
	void Release() override;
	vec3f HoldOffset() const override;

private:

	void CreateBullet(bool const& onBeat, vec2f const& direction) const;

};

#endif
