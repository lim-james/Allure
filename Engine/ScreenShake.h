#ifndef SCREEN_SHAKE_H
#define SCREEN_SHAKE_H

#include "Script.h"

#include <Events/Event.h>

struct ScreenShake : Script {

	float duration;
	float magnitude;

private:

	float k;

	float et;
	vec2f displacement;

	void Awake() override;
	void Update() override;

	void ShakeHandler(Events::Event* event);

};

#endif
