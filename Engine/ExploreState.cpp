#include "ExploreState.h"

#include "StateContainer.h"
#include "TeamContainer.h"
#include "HexMaze.h"

#include <Math/Random.hpp>

void States::Explore::Enter(const unsigned & target, EntityManager * const entities) {

	auto team = entities->GetComponent<TeamContainer>(target)->team;

	// check if there are any positions
	for (auto& unit : team->GetUnits()) {
		if (unit->queuedPositions.empty()) {
			unit->chasing = false;
			std::fill(unit->visited.begin(), unit->visited.end(), false);
			unit->queuedPositions.push(team->GetMaze()->ScreenToMapPosition(unit->transform->translation));
		}
	}
}

void States::Explore::Update(const unsigned & target, const float & dt, EntityManager * const entities) { }

void States::Explore::Exit(const unsigned & target, EntityManager * const entities) {

	auto team = entities->GetComponent<TeamContainer>(target)->team;
	auto maze = team->GetMaze();

	// DFS
	for (auto& unit : team->GetUnits()) {

		auto position = unit->transform->translation.xy;
		auto mapPosition = maze->ScreenToMapPosition(position);

		Scan(team, unit);
		if (unit->queuedPositions.size()) {
			const auto target = unit->queuedPositions.top();
			unit->path = { target };
		}
	}
	//Console::Warn << team->GetQueuedPositions().top() << '\n';
	//Print(team->GetQueuedPositions());


	if (team->GetOpponentUnits().size()) {
		entities->GetComponent<StateContainer>(target)->queuedState = "ATTACK";
	}
}

void States::Explore::Scan(Team * const team, Unit * const unit) {
	bool deadend = true;

	auto& positions = unit->queuedPositions;
	auto maze = team->GetMaze();

	std::map<int, Unit*> selfUnits;
	for (auto& unit : team->GetUnits()) {
		const auto mapPosition = maze->ScreenToMapPosition(unit->transform->translation);
		const auto index = maze->GetMapIndex(mapPosition);
		selfUnits[index] = unit;
	}

	std::vector<unsigned> choices;
	choices.resize(6, 0);
	for (unsigned i = 0; i < 6; ++i) {
		choices[i] = i;
	}

	// check dirs
	while (choices.size()) {
		const unsigned i = Math::RandMinMax((unsigned)0, choices.size());

		auto dir = maze->directions[choices[i]];
		choices.erase(choices.begin() + i);
		// map pos
		const auto newPosition = maze->ScreenToMapPosition(unit->transform->translation.xy + dir);
		const int index = maze->GetMapIndex(newPosition);

		if (index < 0 || unit->visited[index] || team->GetOpponentUnit(index) || selfUnits[index]) continue;

		unit->visited[index] = true;

		if (maze->GetMapData(index) == WALL) {
			continue;
		}

		positions.push(newPosition);

		deadend = false;
		break;
	}

	if (deadend) {
		positions.pop();
	}
}
