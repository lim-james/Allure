#include "AttackState.h"

#include "StateContainer.h"
#include "TeamContainer.h"
#include "Transform.h"

#include <Events/EventsManager.h>

void States::Attack::Enter(const unsigned & target, EntityManager * const entities) {
	auto transform = entities->GetComponent<Transform>(target);
	auto team = entities->GetComponent<TeamContainer>(target)->team;

	std::vector<Unit*> list;

	// check which are in range
	for (auto& unit : team->GetUnits()) {
		if (unit->visibleUnits.size() && unit->canAttack) {
			std::fill(unit->visited.begin(), unit->visited.end(), false);
			while (unit->queuedPositions.size())
				unit->queuedPositions.pop();
			list.push_back(unit);
		}
	}

	// none in range
	if (list.empty()) {
		entities->GetComponent<StateContainer>(target)->queuedState = "IDLE";

		if (team->GetUnits().empty()) return;

		Unit* selected = nullptr;

		for (auto& unit : team->GetUnits()) {
			if (unit->canAttack) {
				selected = unit;
				break;
			}
		}

		team->SelectUnit(selected);

		if (!selected) {
			return;
		}

		Unit* targetUnit = nullptr;
		float distance = -1.f;

		if (selected->chasing) {
			if (selected->path.empty()) {
				selected->chasing = false;
			} else {
				return;
			}
		}

		for (auto& pair : team->GetOpponentUnits()) {
			if (!pair.second) continue;
			const float curr = Math::LengthSquared(pair.second->transform->translation - selected->transform->translation);
			if (targetUnit) {
				if (curr < distance) {
					targetUnit = pair.second;
					distance = curr;
				}
			} else {
				targetUnit = pair.second;
				distance = curr;
			}
		}

		if (!targetUnit) return;

		selected->path = team->GetPath(selected->transform->translation, targetUnit->transform->translation);
		selected->chasing = true;
	} else {
		team->SelectUnit(list.front());
		Unit* const selected = team->GetSelectedUnit();
		Unit* targetUnit = selected->visibleUnits.begin()->second;

		selected->chasing = false;
		
		//Console::Warn << "Attack\n";
		Events::EventsManager::GetInstance()->Trigger("ATTACK", new Events::Attack(team, targetUnit->transform->translation));
		//selected->path = team->GetPath(selected->transform->translation.xy, target->transform->translation.xy);
	}
}

void States::Attack::Update(const unsigned & target, const float & dt, EntityManager * const entities) {
	//auto team = entities->GetComponent<TeamContainer>(target)->team;
	//if (team->GetOpponentUnits().empty()) {
	//	entities->GetComponent<StateContainer>(target)->queuedState = "EXPLORE";
	//}
}

void States::Attack::Exit(const unsigned & target, EntityManager * const entities) {
}
