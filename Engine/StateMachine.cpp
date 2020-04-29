#include "StateMachine.h"

#include <Events/EventsManager.h>
#include <Helpers/VectorHelpers.h>
#include <MACROS.h>

StateMachine::~StateMachine() {
	for (auto& state : states)
		delete state.second;

	states.clear();
}

void StateMachine::Initialize() {
	EventsManager::Get()->Subscribe("STATE_CONTAINER_ACTIVE", &StateMachine::ActiveHandler, this);
}

void StateMachine::Update(float const& dt) {
	for (auto& c : components) {
		if (c->queuedState != "") {
			const auto previous = c->currentState;
			c->currentState = c->queuedState;
			if (previous != "")
				states[previous]->Exit(c->entity, entities);
			c->queuedState = "";
			states[c->currentState]->Enter(c->entity, entities);
		}

		if (c->currentState != "")
			states[c->currentState]->Update(c->entity, dt, entities);
	}
}

void StateMachine::ActiveHandler(Events::Event* event) {
	const auto c = static_cast<Events::AnyType<StateContainer*>*>(event)->data;

	if (c->IsActive())
		Helpers::Insert(components, c);
	else
		Helpers::Remove(components, c);

}
