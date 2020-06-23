#ifndef PLAYER_COMBAT_H
#define PLAYER_COMBAT_H

#include "Script.h"
#include "SpriteAnimation.h"
#include "WeaponBase.h"
#include "Prefab.h"
#include "Text.h"

#include <Handler/Handler.h>

struct PlayerCombat : Script {

	using base_type = PlayerCombat;

	Transform* weaponHolder;
	Prefab* sfxPrefab;
	Text* pickupLabel;

	void SetCrosshair(Transform* const transform);
	void SetHolder(Transform* const transform);

	void OnCollisionEnter(unsigned target);
	void OnCollisionExit(unsigned target);

private:

	SpriteAnimation* spriteAnimation;

	Transform* crosshair;
	Transform* holder;
	WeaponBase* weapon;

	unsigned hovering;

	bool isTriggering;

	void Awake() override;
	void Start() override;
	void Update() override;

	void KeyHandler(Events::Event* event);
	void MouseButtonHandler(Events::Event* event);

	void PickupWeapon();
	void DropWeapon();

};

#endif
