#ifndef STATE_H
#define STATE_H

#include "EntityManager.h"

#include <string>

namespace States {

	struct State {
		virtual void Enter(unsigned const& target, EntityManager * const entities) = 0;
		virtual void Update(unsigned const& target, float const& dt, EntityManager * const entities) = 0;
		virtual void Exit(unsigned const& target, EntityManager * const entities) = 0;
	};

}

#endif
