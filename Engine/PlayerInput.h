#ifndef PLAYER_INPUT_H
#define PLAYER_INPUT_H

#include "Script.h"
#include "Physics.h"

#include <Math/Vectors.h>
#include <Events/Event.h>

struct PlayerInput : Script {

	float speed;

private:

	Physics* physics;
	vec2f direction;

	void Awake() override;
	void Update() override;

	void KeyHandler(Events::Event* event);

};

#endif
