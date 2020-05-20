#ifndef ENEMY_LIFE_H
#define ENEMY_LIFE_H

#include "Script.h"
#include "ParticleEmitter.h"
#include "StateContainer.h"
#include "SpriteRender.h"
#include "Prefab.h"

struct EnemyLife : Script {

	unsigned points;
	int lives;

	void OnCollisionEnter(unsigned target);
	void Kill();

private:

	float flashBt;
	float bt;

	StateContainer* state;
	SpriteRender* render;
	
	void Awake() override;
	void Start() override;
	void Update() override;

	void Hit(bool const& bonus);

};

#endif
