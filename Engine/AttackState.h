#ifndef ATTACK_STATE_H
#define ATTACK_STATE_H

#include "State.h"

#include "Team.h"

#include <Math/Vectors.hpp>
#include <Events/Event.h>

namespace Events {

	struct Attack : Event {
		Team * const team;
		const vec3f position;

		Attack(Team * const team, const vec3f& position)
			: team(team)
			, position(position) {}	
	};

}


namespace States {
	
	struct Attack : State {
		void Enter(const unsigned& target, EntityManager * const entities) override;
		void Update(const unsigned& target, const float& dt, EntityManager * const entities) override;
		void Exit(const unsigned& target, EntityManager * const entities) override;
	};

}

#endif
