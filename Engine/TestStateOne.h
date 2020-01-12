#ifndef TEST_STATE_ONE_H
#define TEST_STATE_ONE_H

#include "State.h"

namespace States {

	struct TestStateOne : State {
		void Enter(const unsigned& target, EntityManager * const entities) override;
		void Update(const unsigned& target, const float& dt, EntityManager * const entities) override;
		void Exit(const unsigned& target, EntityManager * const entities) override;
	};

}

#endif
