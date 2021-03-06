#ifndef ENEMY_TARGET_H
#define ENEMY_TARGET_H

#include "Script.h"

#include "EnemyData.h"
#include "EnemyLife.h"
#include "Physics.h"
#include "SpriteAnimation.h"

#include <Events/Event.h>

struct EnemyTarget : Script {

	using base_type = EnemyTarget;

	float magnitude;

	Transform* player;
	vec2f boundary;

	TargetStyle farStyle;
	TargetStyle nearStyle;
	TargetStyle* style;

	void Chase();
	vec3f GetTarget() const;

private:

	bool isNear;
	vec2f target;

	EnemyLife* life;
	Physics* physics;
	SpriteAnimation* spriteAnimation;

	void Awake() override;
	void Update() override;

	void EventHandler(Events::Event* event);

};

#endif
