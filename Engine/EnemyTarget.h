#ifndef ENEMY_TARGET_H
#define ENEMY_TARGET_H

#include "Script.h"

#include "Physics.h"

struct EnemyTarget : Script {

	float speed;
	Transform* player;

	void Chase();

private:

	Physics* physics;

	void Awake() override;
	void Update() override;

};

#endif
