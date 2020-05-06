#ifndef PLAYER_INPUT_H
#define PLAYER_INPUT_H

#include "Script.h"
#include "ParticleEmitter.h"
#include "Physics.h"

#include <Math/Vectors.h>
#include <Events/Event.h>

struct PlayerInput : Script {

	float speed;
	float dash;

private:

	ParticleEmitter* emitter;
	Physics* physics;

	vec2f axes;
	vec2f direction;
	float dashMagnitude;

	void Awake() override;
	void Update() override;

	void KeyHandler(Events::Event* event);

};

#endif
