#ifndef ATTACK_STATE_H
#define ATTACK_STATE_H

#include "State.h"

namespace States {
	
	struct Attack : State {
		void Enter(const unsigned& target, EntityManager * const entities) override;
		void Update(const unsigned& target, const float& dt, EntityManager * const entities) override;
		void Exit(const unsigned& target, EntityManager * const entities) override;
	};

}

#endif
