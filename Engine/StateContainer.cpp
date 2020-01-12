#include "StateContainer.h"

#include <Events/EventsManager.h>

StateContainer::StateContainer()
	: queuedState("")
	, currentState("") {}

void StateContainer::Initialize() {
	queuedState = "";
	currentState = "";
}

void StateContainer::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("STATE_CONTAINER_ACTIVE", new Events::AnyType<StateContainer*>(this));
}
