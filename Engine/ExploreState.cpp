#include "ExploreState.h"

#include "StateContainer.h"
#include "TeamContainer.h"
#include "HexMaze.h"

void States::Explore::Enter(const unsigned & target, EntityManager * const entities) {
	auto team = entities->GetComponent<TeamContainer>(target)->team;
	auto& positions = team->GetQueuedPositions();

	Unit* scout = nullptr;
	for (auto& unit : team->GetUnits()) {
		if (scout) {
			if (unit->viewRange > scout->viewRange)
				scout = unit;
		} else {
			scout = unit;
		}
	}

	team->SelectUnit(scout);
	auto maze = team->GetMaze();

	if (positions.empty()) {
		positions.push(maze->ScreenToMapPosition(scout->transform->translation));
	} else {

	}
}

void States::Explore::Update(const unsigned & target, const float & dt, EntityManager * const entities) { }

void States::Explore::Exit(const unsigned & target, EntityManager * const entities) {
	auto team = entities->GetComponent<TeamContainer>(target)->team;
	auto scout = team->GetSelectedUnit();
	auto maze = team->GetMaze();
	auto position = scout->transform->translation.xy;
	auto mapPosition = maze->ScreenToMapPosition(position);

	//Console::Warn << team->GetQueuedPositions().top() << '\n';
	//Print(team->GetQueuedPositions());

	Scan(team);
	if (!team->GetQueuedPositions().empty())
		scout->path = { team->GetQueuedPositions().top() };

	if (team->GetOpponentUnits().size()) {
		entities->GetComponent<StateContainer>(target)->queuedState = "ATTACK";
	}
}

void States::Explore::Scan(Team * const team) {
	bool deadend = true;

	auto& positions = team->GetQueuedPositions();
	auto maze = team->GetMaze();
	auto scout = team->GetSelectedUnit();

	for (auto& dir : maze->directions) {
		const auto test = maze->ScreenToMapPosition(scout->transform->translation.xy);
		const auto newPosition = maze->ScreenToMapPosition(scout->transform->translation.xy + dir);
		const int index = maze->GetMapIndex(newPosition);
		//Scan(curr + dir);

		if (index < 0 || team->IsVisited(index)) continue;

		team->SetVisited(index, true);
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
