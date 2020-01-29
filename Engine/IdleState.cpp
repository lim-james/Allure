#include "IdleState.h"

#include "StateContainer.h"
#include "TeamContainer.h"

#include <Logger/Logger.h>

void States::Idle::Enter(const unsigned& target, EntityManager * const entities) {
}

void States::Idle::Update(const unsigned& target, const float& dt, EntityManager * const entities) { 
	auto team = entities->GetComponent<TeamContainer>(target)->team;
	if (team->GetOpponentUnits().size()) {
		entities->GetComponent<StateContainer>(target)->queuedState = "ATTACK";
	} else {
		entities->GetComponent<StateContainer>(target)->queuedState = "EXPLORE";
	}
}

void States::Idle::Exit(const unsigned& target, EntityManager * const entities) {
}
