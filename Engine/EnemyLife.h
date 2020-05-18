#ifndef ENEMY_LIFE_H
#define ENEMY_LIFE_H

#include "Script.h"
#include "ParticleEmitter.h"
#include "Prefab.h"

struct EnemyLife : Script {

	unsigned points;

	void OnCollisionEnter(unsigned target);

};

#endif
