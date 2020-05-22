#ifndef TEXT_BLINK_H
#define TEXT_BLINK_H

#include "Script.h"
#include "Text.h"

struct TextBlink : Script {

	float duration;

private:

	Text* text;
	float bt;

	void Awake() override;
	void Start() override;
	void Update() override;

};

#endif
