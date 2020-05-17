#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "System.h"
#include "StateContainer.h"

#include "State.h"

#include <Events/Event.h>

#include <vector>
#include <map>

class StateMachine : public System {

	std::map<std::string, States::State*> states;
	std::vector<StateContainer*> components;

public:

	~StateMachine() override;

	void Initialize() override;
	void Update(float const& dt) override;
	void FixedUpdate(float const& dt) override;

	template<typename State>
	void AttachState(std::string const& key);

private:

	void ActiveHandler(Events::Event* event);

};


template<typename State>
void StateMachine::AttachState(std::string const& key) {
	states[key] = new State;
}

#endif
