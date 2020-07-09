#ifndef BUTTON_ANIMATION_H
#define BUTTON_ANIMATION_H

#include "Script.h"

// components
#include "SpriteRender.h"
#include "Animator.h"
#include "Button.h"

struct ButtonAnimation : Script {

	using base_type = ButtonAnimation;

private:

	SpriteRender* render;
	Animator* animation;
	Button* button;

	void Awake() override;

	void OnMouseOver(unsigned target);
	void OnMouseOut(unsigned target);
	void OnMouseDown(unsigned target);
	void OnMouseUp(unsigned target);

};

#endif
