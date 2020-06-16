#ifndef PISTOL_SCRIPT_H
#define PISTOL_SCRIPT_H

#include "WeaponBase.h"
#include "Prefab.h"

struct PistolScript : WeaponBase {

	Prefab* standardPrefab;
	Prefab* explosivePrefab;

	void Trigger() override;
	void Hold(float const& dt) override;
	void Release() override;
	vec3f HoldOffset() const override;

private:

	void CreateBullet(bool const& onBeat) const;

};

#endif
