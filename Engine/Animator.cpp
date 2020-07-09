#include "Animator.h"

#include <Events/EventsManager.h>

Animator::~Animator() {
	Clear();
}

void Animator::Initialize() {
	Clear();
}

Component * Animator::Clone() const {
	return new Animator(*this);
}

void Animator::SetActive(bool const& state) {
	Component::SetActive(state);
	EventsManager::Get()->Trigger("ANIMATOR_ACTIVE", new Events::AnyType<Animator*>(this));
}

void Animator::Clear() {
	for (auto& list : animations)
		for (AnimationBase* const a : list.second)
			delete a;

	animations.clear();
}

