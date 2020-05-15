#include "Animation.h"

#include <Events/EventsManager.h>

AnimationBase::AnimationBase(bool const& loop, float const& duration, float const& delay, Handler<void,void> completion)
	: loop(loop)
	, duration(duration)
	, delay(delay)
	, et(0.f)
	, completion(completion) {}

void AnimationBase::Update(float const& dt) {
	if (delay > 0.f)
		delay -= dt;
	else
		et += dt;
}

const bool AnimationBase::IsActive() const {
	return loop || et < duration;
}

float const & AnimationBase::GetDuration() const {
	return duration;
}

float const & AnimationBase::GetDelay() const {
	return delay;
}

Animation::Animation() {}

Animation::~Animation() {
	Clear();
}

void Animation::Initialize() {
	Clear();
}

Component * Animation::Clone() const {
	return new Animation(*this);
}

void Animation::SetActive(bool const& state) {
	Component::SetActive(state);
	EventsManager::Get()->Trigger("ANIMATION_ACTIVE", new Events::AnyType<Animation*>(this));
}

void Animation::Clear() {
	for (auto& list : animations)
		for (AnimationBase* const a : list.second)
			delete a;

	animations.clear();
}

