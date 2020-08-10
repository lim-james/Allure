#ifndef ENEMY_COMBAT_H
#define ENEMY_COMBAT_H

#include "Script.h"

#include "EnemyLife.h"
#include "EnemyTarget.h"
#include "WeaponBase.h"

#include <Events/Event.h>

struct EnemyCombat : Script {

	using base_type = EnemyCombat;

	int beatStride;

	float holdDuration;
	Transform* weaponHolder;

	void SetWeapon(WeaponBase* const ptr);

private:

	float bt;
	bool isBeat;

	int beatBounce;

	EnemyLife* life;
	EnemyTarget* target;
	WeaponBase* weapon;

	void Awake() override;
	void Start() override;
	void Update() override;

	void BeatHandler();

};

#endif
