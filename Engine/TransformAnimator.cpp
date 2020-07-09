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

void TransformAnimator::Queue(AnimationBase const & base, short const & target, vec3f const & outcome) {
	animations[target].push_back(TransformAnimationData(base, 0.f, outcome));
}

unsigned TransformAnimator::Count(short const & target) const {
	return animations[target].size();
}

void TransformAnimator::Pop(short const & target) {
	animations[target].pop_back();
}

void TransformAnimator::Clear(short const & target) {
	animations[target].clear();
}

void TransformAnimator::Clear() {
	animations[ANIMATE_SCALE].clear();	
	animations[ANIMATE_ROTATION].clear();	
	animations[ANIMATE_TRANSLATION].clear();	
}

