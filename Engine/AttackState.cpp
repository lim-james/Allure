#include "AttackState.h"

#include "StateContainer.h"
#include "TeamContainer.h"
#include "Transform.h"

void States::Attack::Enter(const unsigned & target, EntityManager * const entities) {
	auto transform = entities->GetComponent<Transform>(target);
	auto team = entities->GetComponent<TeamContainer>(target)->team;

	std::vector<Unit*> list;

	for (auto& unit : team->GetUnits()) {
		if (unit->visibleUnits.size()) {
			list.push_back(unit);
		}
	}
}

void States::Attack::Update(const unsigned & target, const float & dt, EntityManager * const entities) {
	auto team = entities->GetComponent<TeamContainer>(target)->team;
	if (team->GetOpponentUnits().empty()) {
		entities->GetComponent<StateContainer>(target)->queuedState = "EXPLORE";
	}
}

void States::Attack::Exit(const unsigned & target, EntityManager * const entities) {
}
