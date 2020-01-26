#include "Team.h"

#include "HexMaze.h"
#include "Animation.h"

#include <Helpers/VectorHelpers.h>

Team::Team() 
	: maze(nullptr)
	, selected(nullptr) {}

void Team::SetName(const std::string & _name) {
	name = _name;
}

const std::string& Team::GetName() const {
	return name;
}

void Team::SetMaze(HexGrid * const _maze) {
	std::fill(vision.begin(), vision.end(), false);
	std::fill(visited.begin(), visited.end(), false);
	maze = _maze;
}

void Team::SetVision(const unsigned & size) {
	vision.resize(size * size, false);
	visited.resize(size * size, false);
}

const std::vector<bool>& Team::GetVision() const {
	return vision;
}

bool Team::InSight(const vec2f & screenPosition) const {
	const auto map = maze->ScreenToMapPosition(screenPosition);
	const auto index = maze->GetMapIndex(map);
	return vision[index];
}

void Team::AddUnit(Unit * const unit) {
	units.push_back(unit);
	Scan(unit);
}

const std::vector<Unit*>& Team::GetUnits() const {
	return units;
}

Unit * const Team::GetUnitAt(const vec3f & position) const {
	for (auto& unit : units) {
		if (unit->transform->translation.xy == position.xy) {
			return unit;
		}
	}

	return nullptr;
}

bool Team::IsDead() const {
	return units.empty();
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

bool Team::DestroyUnit(const unsigned & entity) {
	for (unsigned i = 0; i < units.size(); ++i) {
		if (units[i]->transform->entity == entity) {
			if (selected == units[i])
				selected = nullptr;
			units.erase(units.begin() + i);
			return true;
		}
	}
	return false;
}

bool Team::DestroyUnit(Unit * const unit) {
	if (Helpers::Remove(units, unit)) {
		if (selected == unit)
			selected = nullptr;
		return true;
	}
	return false;
}

void Team::DestroyUnits(const std::function<void(unsigned)>& completion) {
	for (auto& unit : units) {
		completion(unit->transform->entity);
		delete unit;
	}
	units.clear();
}

std::vector<vec2i> Team::GetPath(const vec2f & start, const vec2f & end) const {
	const auto position = maze->ScreenToMapPosition(start);
	const auto target = maze->ScreenToMapPosition(end);

	std::vector<DNode*> opened;
	std::vector<DNode*> closed;

	DNode* current = new DNode;
	current->g = 0;
	current->h = Math::LengthSquared<float>(target - position);
	current->position = position;
	current->previous = nullptr;

	opened.push_back(current);

	while (!opened.empty()) {
		DNode* best = opened.front();
		opened.erase(opened.begin());
		closed.push_back(best);

		if (best->position == target)
			break;

		const vec2f screenPosition = maze->MapToScreenPosition(best->position);

		for (auto& dir : maze->directions) {
			const vec2i pos = maze->ScreenToMapPosition(dir + screenPosition);

			if (maze->GetMapData(pos) <= 0 || !vision[maze->GetMapIndex(pos)]) continue;

			bool found = false;
			for (auto& node : closed) {
				if (node->position == pos) {
					found = true;
					break;
				}
			}

			if (found) continue;

			unsigned i = 0;
			for (; i < opened.size(); ++i) {
				if (opened[i]->position == pos) {
					break;
				}
			}

			const float weight = static_cast<float>(maze->GetMapData(maze->GetMapIndex(pos)));

			DNode* neighbour = new DNode;
			neighbour->g = best->g + 1;
			neighbour->h = Math::LengthSquared<float>(pos - target) * weight;
			neighbour->position = pos;
			neighbour->previous = best;

			if (i != opened.size()) {
				if (opened[i]->f() > neighbour->f()) {
					delete opened[i];
					opened[i] = neighbour;
				} else {
					delete neighbour;
				}
			} else {
				i = 0;
				for (; i < opened.size(); ++i) {
					if (opened[i]->f() > neighbour->f()) {
						opened.insert(opened.begin() + i, neighbour);
						break;
					}
				}

				if (i == opened.size()) {
					opened.insert(opened.begin() + i, neighbour);
					opened.push_back(neighbour);
				}
			}
		}
	}

	std::vector<vec2i> path;

	auto last = closed.back();
	while (last) {
		for (int i = 0; i < maze->GetMapData(maze->GetMapIndex(last->position)); ++i) {
			path.insert(path.begin(), last->position);
		}
		last = last->previous;
	}

	return path;
}

bool Team::Move(const float& dt, EntityManager * const entities) {
	bool moved = false;

	for (auto& unit : units) {
		if (!unit->path.empty()) {
			entities->GetComponent<Animation>(unit->transform->entity)->Animate(
				AnimationBase(false, dt * 0.9f),
				unit->transform->translation.xy,
				maze->MapToScreenPosition(unit->path.front())
			);
			unit->path.erase(unit->path.begin());
			Scan(unit);
			moved = true;
		}
	}

	return moved;
}

void Team::Scan(Unit * const unit) {
	bool deadend = true;

	unit->vision.clear();

	const vec2f& position = unit->transform->translation.xy;

	const auto outerPositions = maze->GetTilesAtRange(unit->viewRange, position);
	for (auto& pos : outerPositions) {
		const auto dir = (pos - position) / unit->viewRange;

		for (float i = 0.f; i <= unit->viewRange; ++i) {
			const auto target = maze->ScreenToMapPosition(position + dir * i);
			const int index = maze->GetMapIndex(target);

			if (index < 0) break;

			unit->vision.push_back(index);
			vision[index] = true;

			if (maze->GetMapData(index) == WALL) break;
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
