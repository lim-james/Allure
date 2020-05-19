#ifndef PLAYER_COMBAT_H
#define PLAYER_COMBAT_H

#include "Script.h"
#include "SpriteAnimation.h"
#include "WeaponBase.h"

#include <Handler/Handler.h>

struct PlayerCombat : Script {

	Transform* weaponHolder;

	void SetCrosshair(Transform* const transform);
	void SetWeapon(WeaponBase* const ptr);

private:

	SpriteAnimation* spriteAnimation;

	Transform* crosshair;
	WeaponBase* weapon;

	bool isTriggering;

	void Awake() override;
	void Update() override;

	void MouseButtonHandler(Events::Event* event);

};

#endif
