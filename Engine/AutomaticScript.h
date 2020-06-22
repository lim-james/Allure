#ifndef AUTOMATIC_SCRIPT_H
#define AUTOMATIC_SCRIPT_H

#include "WeaponBase.h"

struct AutomaticScript : WeaponBase {

	using base_type = WeaponBase;
	
	AutomaticScript();

	void Trigger() override;
	void Hold(float const& dt) override;
	void Release() override;
	vec3f HoldOffset() const override;
	std::string Name() const override;

private:

	bool onBeat;
	float firerate;
	float bt;

	unsigned maxBurst;
	unsigned fireCount;

	void CreateBullet(bool const& onBeat, vec2f const& direction) const;

};

#endif
