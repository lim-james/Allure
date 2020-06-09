#ifndef ENEMY_TARGET_H
#define ENEMY_TARGET_H

#include "Script.h"

#include "Physics.h"
#include <Events/Event.h>

struct EnemyTarget : Script {

	float magnitude;

	float speed;
	float minRadius;
	Transform* player;

	void Chase();

private:

	Physics* physics;

	void Awake() override;
	void Update() override;

	void EventHandler(Events::Event* event);

};

#endif
