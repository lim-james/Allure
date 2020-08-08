#ifndef AV_BUTTON_HANDLER_H
#define AV_BUTTON_HANDLER_H

#include "Script.h"

#include "Animator.h"
#include "TransformAnimator.h"
#include "SpriteRender.h"

struct AVButtonHandler : Script {

	using base_type = AVButtonHandler;

	Animator* animator;
	TransformAnimator* tAnimator;

	SpriteRender* outline;

	void Start() override;

	void OnMouseOver(unsigned target);
	void OnMouseOut(unsigned target);
	void OnMouseDown(unsigned target);
	void OnMouseUp(unsigned target);

private:

	SpriteRender* render;
	Transform* outlineTransform;

};

#endif
