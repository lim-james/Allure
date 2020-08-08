#include "TransformAnimator.h"

#include <Events/EventsManager.h>

TransformAnimator::~TransformAnimator() {
	Clear();
}

void TransformAnimator::Initialize() {
	Clear();
}

Component * TransformAnimator::Clone() const {
	return new TransformAnimator(*this);
}

void TransformAnimator::SetActive(bool const & state) {
	Component::SetActive(state);
	EventsManager::Get()->Trigger("TRANSFORM_ANIMATOR_ACTIVE", new Events::AnyType<TransformAnimator*>(this));
}

void TransformAnimator::Queue(AnimationBase const & base, Transform* const target, short const& attribute, vec3f const & outcome) {
	animations[attribute][target].push_back(TransformAnimationData(base, 0.f, outcome));
}

unsigned TransformAnimator::Count(Transform* const target, short const & attribute) const {
	return animations[attribute].at(target).size();
}

void TransformAnimator::Pop(Transform* const target, short const & attribute) {
	animations[attribute][target].pop_back();
}

void TransformAnimator::Clear(Transform* const target, short const & attribute) {
	animations[attribute][target].clear();
}

void TransformAnimator::Clear() {
	animations[ANIMATE_SCALE].clear();	
	animations[ANIMATE_ROTATION].clear();	
	animations[ANIMATE_TRANSLATION].clear();	
}

