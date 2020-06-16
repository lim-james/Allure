#ifndef AUTOMATIC_SCRIPT_H
#define AUTOMATIC_SCRIPT_H

#include "WeaponBase.h"
#include "Prefab.h"

struct AutomaticScript : WeaponBase {

	Prefab* standardPrefab;
	Prefab* explosivePrefab;
	
	AutomaticScript();

	void Trigger() override;
	void Hold(float const& dt) override;
	void Release() override;
	vec3f HoldOffset() const override;

private:

	bool onBeat;
	float firerate;
	float bt;

	unsigned maxBurst;
	unsigned fireCount;

	void CreateBullet(bool const& onBeat) const;

};

#endif
