#ifndef STATE_CONTAINER_H
#define STATE_CONTAINER_H

#include "Component.h"

#include <string>

struct StateContainer : Component {

	std::string queuedState;
	std::string currentState;

	StateContainer();

	void Initialize() override;
	void SetActive(const bool& state) override;

};

#endif
