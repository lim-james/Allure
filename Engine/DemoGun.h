#ifndef DEMO_GUN_H
#define DEMO_GUN_H

#include "WeaponBase.h"

class DemoGun : public WeaponBase {

	float firerate;
	float bt;

public:
	
	DemoGun();

	void Trigger() override;
	void Hold(float const& dt) override;
	void Release() override;

private:

	void CreateBullet() const;

};


#endif
