#include "Team.h"

#include "HexMaze.h"

#include <Logger/Logger.h>

Team::Team() 
	: maze(nullptr)
	, vision(nullptr)
	, selected(nullptr) {}

void Team::SetMaze(HexGrid * const _maze) {
	maze = _maze;
}

void Team::SetVision(const unsigned & size) {
	vision = new HexGrid(size, FOG);
	visited.resize(size * size, false);
}

HexGrid * const Team::GetVision() const {
	return vision;
}

bool Team::InSight(const vec2f & position) const {
	const auto map = vision->ScreenToMapPosition(position);
	const auto index = vision->GetMapIndex(map);
	return vision->GetMapData(index) != FOG;
}

void Team::AddUnit(Unit * const unit) {
	units.push_back(unit);
	Scan(unit);
}

bool Team::SelectUnit(const unsigned & entity) {
	for (auto& unit : units) {
		if (unit->transform->entity == entity) {
			selected = unit;
			return true;
		}
	}

	return false;
}

Unit * const Team::GetSelectedUnit() const {
	return selected;
}

bool Team::Move() {
	bool moved = false;

	for (auto& unit : units) {
		if (!unit->path.empty()) {

			unit->transform->translation.xy = vision->MapToScreenPosition(unit->path.front());
			unit->path.erase(unit->path.begin());
			Scan(unit);
			moved = true;
		}
	}

	return moved;
}

void Team::Scan(Unit * const unit) {
	bool deadend = true;

	const vec2f& position = unit->transform->translation.xy;

	const auto outerPositions = vision->GetTilesAtRange(unit->range, position);
	for (auto& pos : outerPositions) {
		const auto dir = (pos - position) / unit->range;

		for (float i = 1.f; i <= unit->range; ++i) {
			const auto target = vision->ScreenToMapPosition(position + dir * i);
			const int index = vision->GetMapIndex(target);

			if (index < 0) break;

			const int tile = maze->GetMapData(index);
			vision->SetMapData(index, tile);

			if (tile == WALL) break;
		}
	}

	//for (auto& dir : directions) {
	//	const auto newPosition = maze->ScreenToMapPosition(screenPosition + dir);
	//	const int index = maze->GetMapIndex(newPosition);
	//	//Scan(curr + dir);
	//	if (index < 0 || visited[index]) continue;

	//	visited[index] = true;

	//	const bool isBlocked = maze->IsBlocked(index);
	//	if (isBlocked) {
	//		vision[index] = WALL;
	//		continue;
	//	}

	//	vision[index] = maze->GetTileType(index);
	//	queuedPositions.push(newPosition);
	//	deadend = false;
	//}

	//if (deadend)
	//	queuedPositions.pop();
}
