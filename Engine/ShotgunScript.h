#ifndef SHOTGUN_SCRIPT_H
#define SHOTGUN_SCRIPT_H

#include "WeaponBase.h"
#include "Prefab.h"

struct ShotgunScript : WeaponBase {

	Prefab* standardPrefab;
	Prefab* explosivePrefab;

	void Trigger() override;
	void Hold(float const& dt) override;
	void Release() override;
	vec3f HoldOffset() const override;

private:

	void CreateBurst(bool const& onBeat) const;

};


#endif
