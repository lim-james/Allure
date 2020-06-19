#ifndef ENEMY_LIFE_H
#define ENEMY_LIFE_H

#include "Script.h"
#include "ParticleEmitter.h"
#include "StateContainer.h"
#include "SpriteRender.h"
#include "Physics.h"
#include "Prefab.h"

struct EnemyLife : Script {

	int shield;
	int health;
	unsigned points;
	bool bodyDamage;

	void OnCollisionEnter(unsigned target);
	void Kill();

	bool IsStunned() const;

private:

	bool hasFroze;

	float flashDelay, stunDelay;
	float flashBt, stunBt;

	StateContainer* state;
	SpriteRender* render;
	Physics* physics;

	void Awake() override;
	void Start() override;
	void Update() override;

	void Hit(unsigned const& target, bool const& bonus);

};

#endif
