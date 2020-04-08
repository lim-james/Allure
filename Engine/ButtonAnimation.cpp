#include "ButtonAnimation.h"

void ButtonAnimation::Awake() {
	render = GetComponent<SpriteRender>();
	animation = GetComponent<Animation>();
	button = GetComponent<Button>();

	button->handlers[MOUSE_OVER].Bind(&ButtonAnimation::OnMouseOver, this);
	button->handlers[MOUSE_OUT].Bind(&ButtonAnimation::OnMouseOut, this);
	button->handlers[MOUSE_DOWN].Bind(&ButtonAnimation::OnMouseDown, this);
	button->handlers[MOUSE_UP].Bind(&ButtonAnimation::OnMouseUp, this);
}

void ButtonAnimation::OnMouseOver(unsigned target) {
	animation->Animate(
		AnimationBase(false, 0.2f),
		&render->tint.a,
		1.f
	);
}

void ButtonAnimation::OnMouseOut(unsigned target) {
	animation->Animate(
		AnimationBase(false, 0.2f),
		&render->tint.a,
		0.5f
	);
}

void ButtonAnimation::OnMouseDown(unsigned target) {
	animation->Animate(
		AnimationBase(false, 0.2f),
		&transform->scale,
		vec3f(7.f, 1.5f, 0.f) * 0.9f
	);
}

void ButtonAnimation::OnMouseUp(unsigned target) {
	animation->Animate(
		AnimationBase(false, 0.2f),
		&transform->scale,
		vec3f(7.f, 1.5f, 0.f)
	);
}
