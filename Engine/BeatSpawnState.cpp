#include "BeatSpawnState.h"

#include "Transform.h"
#include "TransformAnimator.h"
#include "Animator.h"
#include "SpriteRender.h"
#include "StateContainer.h"
#include "BeatCollisionHandler.h"

void States::BeatSpawn::Enter(unsigned const & target, EntityManager * const entities) {
	BeatCollisionHandler* const handler = entities->GetComponent<BeatCollisionHandler>(target);

	Transform* const transform = entities->GetComponent<Transform>(target);
	const vec3f targetScale = transform->GetScale();
	transform->SetScale(targetScale * 5.f);

	TransformAnimator* const tAnimator = entities->GetComponent<TransformAnimator>(target);
	tAnimator->Queue(AnimationBase(false, handler->fadeInDuration), ANIMATE_SCALE, targetScale);

	StateContainer* const container = entities->GetComponent<StateContainer>(target);
	SpriteRender* const render = entities->GetComponent<SpriteRender>(target);
	Animator* const animator = entities->GetComponent<Animator>(target);
	animator->Queue(AnimationBase(false, handler->fadeInDuration, 0.f, Handler<void, void>([container]() {
		container->queuedState = "BEAT_NEUTRAL";
	})), &render->tint.a, 1.f);
}

void States::BeatSpawn::Update(unsigned const & target, float const & dt, EntityManager * const entities) {}

void States::BeatSpawn::FixedUpdate(unsigned const & target, float const & dt, EntityManager * const entities) {}

void States::BeatSpawn::Exit(unsigned const & target, EntityManager * const entities) {}
