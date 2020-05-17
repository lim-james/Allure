#ifndef ENEMY_LIFE_H
#define ENEMY_LIFE_H

#include "Script.h"
#include "ParticleEmitter.h"

struct EnemyLife : Script {

	void OnCollisionEnter(unsigned target);

};

#endif
