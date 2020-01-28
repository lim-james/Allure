#ifndef IDLE_STATE_H
#define IDLE_STATE_H

#include "State.h"

namespace States {
	
	struct Idle : State {
		void Enter(const unsigned& target, EntityManager * const entities) override;
		void Update(const unsigned& target, const float& dt, EntityManager * const entities) override;
		void Exit(const unsigned& target, EntityManager * const entities) override;
	};

}

#endif
