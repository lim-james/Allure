#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include "Script.h"
#include "Text.h"

struct FPSCounter : Script {

private:

	Text* text;

	void Awake() override;
	void Update() override;

};

#endif
