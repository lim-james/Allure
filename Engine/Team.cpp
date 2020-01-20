#include "Team.h"

#include "HexMaze.h"

#include <Logger/Logger.h>

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

void Team::AddUnit(Unit * const unit) {
	units.push_back(unit);
	Scan(unit);
}

void Team::Scan(Unit * const unit) {
	bool deadend = true;

	const vec2f& position = unit->transform->translation.xy;

	Console::Warn << position << "   " << vision->ScreenToMapPosition(position) << "\n\n";

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
