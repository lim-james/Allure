#include "Animation.h"

#include <Events/EventsManager.h>

AnimationBase::AnimationBase(const bool& loop, const float& duration, const float& delay, Handler<void,void> completion)
	: loop(loop)
	, duration(duration)
	, delay(delay)
	, et(0.f)
	, completion(completion) {}

void AnimationBase::Update(const float& dt) {
	if (delay > 0.f)
		delay -= dt;
	else
		et += dt;
}

const bool AnimationBase::IsActive() const {
	return loop || et < duration;
}

Animation::Animation() {}

Animation::~Animation() {
	Clear();
}

void Animation::Initialize() {
	Clear();
}

void Animation::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("ANIMATION_ACTIVE", new Events::AnyType<Animation*>(this));
}

void Animation::Clear() {
	for (auto& a : animations)
		delete a.second;

	animations.clear();
}

