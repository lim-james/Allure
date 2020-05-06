#ifndef PLAYER_INPUT_H
#define PLAYER_INPUT_H

#include "Script.h"

#include <Math/Vectors.h>
#include <Events/Event.h>

struct PlayerInput : Script {

	float speed;

private:

	vec2f direction;
	vec2f velocity;

	void Awake() override;
	void Update() override;

	void KeyHandler(Events::Event* event);

};

#endif
