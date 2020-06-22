#ifndef SHOTGUN_SCRIPT_H
#define SHOTGUN_SCRIPT_H

#include "WeaponBase.h"

struct ShotgunScript : WeaponBase {

	using base_type = WeaponBase;

	void Trigger() override;
	void Hold(float const& dt) override;
	void Release() override;
	vec3f HoldOffset() const override;

private:

	void CreateBurst(bool const& onBeat, vec2f const& facing) const;

};


#endif
