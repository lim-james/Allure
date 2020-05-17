#ifndef PLAYER_COMBAT_H
#define PLAYER_COMBAT_H

#include "Script.h"
#include "WeaponBase.h"
#include "BackgroundMaterial.h"
#include "Prefab.h"

#include <Handler/Handler.h>

struct PlayerCombat : Script {
	
	// beat attributes

	Material::Background* material;
	Prefab* indicatorPrefab;

	float threshold;

	void SetTempo(unsigned const& tempo);

	// weapon methods

	void SetCrosshair(Transform* const transform);
	void SetWeapon(WeaponBase* const ptr);

private:

	// beat attributes

	float delay;
	float bt;

	bool isHit;

	// weapon attributes

	Transform* crosshair;
	WeaponBase* weapon;

	bool isTriggering;

	void Awake() override;
	void Start() override;
	void Update() override;

	void MouseButtonHandler(Events::Event* event);

	bool Hit();

};

#endif
