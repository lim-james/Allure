#ifndef STATE_H
#define STATE_H

#include "EntityManager.h"

#include <string>

namespace States {

	struct State {
		virtual void Enter(const unsigned& target, EntityManager * const entities) = 0;
		virtual void Update(const unsigned& target, const float& dt, EntityManager * const entities) = 0;
		virtual void Exit(const unsigned& target, EntityManager * const entities) = 0;
	};

}

#endif
