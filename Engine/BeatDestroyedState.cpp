#include "BeatDestroyedState.h"

#include "Transform.h"
#include "TransformAnimator.h"
#include "Animator.h"
#include "SpriteRender.h"
#include "StateContainer.h"
#include "BeatCollisionHandler.h"

void States::BeatDestroyed::Enter(unsigned const & target, EntityManager * const entities) {	
	BeatCollisionHandler* const handler = entities->GetComponent<BeatCollisionHandler>(target);

	Transform* const transform = entities->GetComponent<Transform>(target);
	const vec3f targetScale = transform->GetScale() * 10.f;

	TransformAnimator* const tAnimator = entities->GetComponent<TransformAnimator>(target);
	tAnimator->Queue(AnimationBase(false, handler->fadeOutDuration), ANIMATE_SCALE, targetScale);

	SpriteRender* const render = entities->GetComponent<SpriteRender>(target);
	render->SetMaterial(handler->material);
	render->tint.a = 0.5f;

	Animator* const animator = entities->GetComponent<Animator>(target);
	animator->Queue(AnimationBase(false, handler->fadeOutDuration, 0.f, Handler<void, void>([entities, target]() {
		entities->Destroy(target);
	})), &render->tint.a, 0.f);
}

void States::BeatDestroyed::Update(unsigned const & target, float const & dt, EntityManager * const entities) {}

void States::BeatDestroyed::FixedUpdate(unsigned const & target, float const & dt, EntityManager * const entities) {}

void States::BeatDestroyed::Exit(unsigned const & target, EntityManager * const entities) {}
