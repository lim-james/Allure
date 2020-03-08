#ifndef BUTTON_ANIMATION_H
#define BUTTON_ANIMATION_H

#include "Script.h"

// components
#include "Render.h"
#include "Animation.h"
#include "Button.h"

struct ButtonAnimation : Script {

private:

	Render* render;
	Animation* animation;
	Button* button;

	void Awake() override;

	void OnMouseOver(unsigned target);
	void OnMouseOut(unsigned target);
	void OnMouseDown(unsigned target);
	void OnMouseUp(unsigned target);

};

#endif
