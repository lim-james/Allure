#ifndef ENEMY_COMBAT_H
#define ENEMY_COMBAT_H

#include "Script.h"

#include "EnemyTarget.h"
#include "WeaponBase.h"

#include <Events/Event.h>

struct EnemyCombat : Script {

	float holdDuration;
	Transform* weaponHolder;

	void SetWeapon(WeaponBase* const ptr);

private:

	float bt;

	EnemyTarget* target;
	WeaponBase* weapon;

	void Awake() override;
	void Start() override;
	void Update() override;

	void BeatHandler();

};

#endif