#include "AVButtonHandler.h"

void AVButtonHandler::Start() {
	render = GetComponent<SpriteRender>();
	render->tint.a = 0.25f;

	outline->tint.a = 0.25f;
	outlineTransform = entities->GetComponent<Transform>(outline->entity);
}

void AVButtonHandler::OnMouseOver(unsigned target) {
	animator->Clear(&render->tint.a);
	animator->Queue(AnimationBase(false, 0.1f), &render->tint.a, 1.f);
	animator->Clear(&outline->tint.a);
	animator->Queue(AnimationBase(false, 0.1f), &outline->tint.a, 1.f);
}

void AVButtonHandler::OnMouseOut(unsigned target) {
	animator->Clear(&render->tint.a);
	animator->Queue(AnimationBase(false, 0.1f), &render->tint.a, 0.25f);
	animator->Clear(&outline->tint.a);
	animator->Queue(AnimationBase(false, 0.1f), &outline->tint.a, 0.25f);
}

void AVButtonHandler::OnMouseDown(unsigned target) {
	tAnimator->Clear(transform, ANIMATE_SCALE);
	tAnimator->Queue(AnimationBase(false, 0.1f), transform, ANIMATE_SCALE, vec3f(0.75f));
	tAnimator->Clear(outlineTransform, ANIMATE_SCALE);
	tAnimator->Queue(AnimationBase(false, 0.1f), outlineTransform, ANIMATE_SCALE, vec3f(1.75f));
}

void AVButtonHandler::OnMouseUp(unsigned target) {
	tAnimator->Clear(transform, ANIMATE_SCALE);
	tAnimator->Queue(AnimationBase(false, 0.1f), transform, ANIMATE_SCALE, vec3f(1.f));
	tAnimator->Clear(outlineTransform, ANIMATE_SCALE);
	tAnimator->Queue(AnimationBase(false, 0.1f), outlineTransform, ANIMATE_SCALE, vec3f(2.f));
}
