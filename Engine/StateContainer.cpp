#include "StateContainer.h"

#include <Events/EventsManager.h>

StateContainer::StateContainer()
	: queuedState("")
	, currentState("") {}

void StateContainer::Initialize() {
	queuedState = "";
	currentState = "";
}

Component * StateContainer::Clone() const {
	return new StateContainer(*this);
}

void StateContainer::SetActive(bool const& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("STATE_CONTAINER_ACTIVE", new Events::AnyType<StateContainer*>(this));
}
