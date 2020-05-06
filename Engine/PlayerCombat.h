#ifndef PLAYER_COMBAT_H
#define PLAYER_COMBAT_H

#include "Script.h"
#include "WeaponBase.h"

struct PlayerCombat : Script {

	void SetCrosshair(Transform* const transform);
	void SetWeapon(WeaponBase* const ptr);

private:

	Transform* crosshair;
	WeaponBase* weapon;

	bool isTriggering;

	void Awake() override;
	void Update() override;

	void MouseButtonHandler(Events::Event* event);

};

#endif
