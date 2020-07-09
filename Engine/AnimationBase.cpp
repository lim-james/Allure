#include "AnimationBase.h"

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
