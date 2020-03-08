#include "ButtonAnimation.h"

void ButtonAnimation::Awake() {
	render = GetComponent<Render>();
	animation = GetComponent<Animation>();
	button = GetComponent<Button>();

	button->BindHandler(MOUSE_OVER, &ButtonAnimation::OnMouseOver, this);
	button->BindHandler(MOUSE_OUT, &ButtonAnimation::OnMouseOut, this);
	button->BindHandler(MOUSE_DOWN, &ButtonAnimation::OnMouseDown, this);
	button->BindHandler(MOUSE_UP, &ButtonAnimation::OnMouseUp, this);
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
