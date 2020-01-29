#include "Team.h"

#include "HexMaze.h"
#include "Animation.h"
#include "StateContainer.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>

Team::Team() 
	: maze(nullptr)
	, selected(nullptr) { }

void Team::SetOpponent(Team * const _opponent) {
	opponent = _opponent;
}

void Team::SetAI(const unsigned & id) {
	ai = id;
}

unsigned Team::GetAI() const {
	return ai;
}

bool Team::IsAI() const {
	return ai;
}

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

HexGrid * const Team::GetMaze() const {
	return maze;
}

std::stack<vec2i>& Team::GetQueuedPositions() {
	return queuedPositions;
}

void Team::SetVision(const unsigned & size) {
	vision.resize(size * size, false);
	visited.resize(size * size, false);
}

const std::vector<bool>& Team::GetVision() const {
	return vision;
}

void Team::UpdateVision() {
	opponentUnits.clear();

	for (auto& self : units) {
		self->visibleUnits.clear();
		for (auto& unit : opponent->GetUnits()) {
			const auto mapPosition = maze->ScreenToMapPosition(unit->transform->translation.xy);
			const auto index = maze->GetMapIndex(mapPosition);

			if (vision[index]) {
				opponentUnits[index] = unit;
			}

			if (Helpers::Contains(self->vision, index)) {
				self->visibleUnits[index] = unit;
			}
		}
	}
}

Unit * const Team::GetOpponentUnit(const unsigned & index) {
	return opponentUnits.find(index) == opponentUnits.end() ? nullptr : opponentUnits[index];
}

std::map<unsigned, Unit*>& Team::GetOpponentUnits() {
	return opponentUnits;
}

bool Team::IsVisited(const unsigned & index) const {
	return visited[index];
}

void Team::SetVisited(const unsigned & index, const bool & visibility) {
	visited[index] = visibility;
}

bool Team::InSight(const vec2f & screenPosition) const {
	const auto map = maze->ScreenToMapPosition(screenPosition);
	const auto index = maze->GetMapIndex(map);
	return vision[index];
}

void Team::AddUnit(Unit * const unit) {
	units.push_back(unit);
	Scan(unit, unit->transform->translation.xy);
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

bool Team::SelectUnit(Unit * const unit) {
	selected = unit;
	return true;
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
	selected = nullptr;
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

	std::map<int, Unit*> unitIndexes;

	for (auto& u : units) {
		auto map = maze->ScreenToMapPosition(u->transform->translation);
		unitIndexes[maze->GetMapIndex(map)] = u;
	}

	for (auto& u : opponent->units) {
		auto map = maze->ScreenToMapPosition(u->transform->translation);
		unitIndexes[maze->GetMapIndex(map)] = u;
	}

	while (!opened.empty()) {
		DNode* best = opened.front();
		opened.erase(opened.begin());
		closed.push_back(best);

		if (best->position == target)
			break;

		const vec2f screenPosition = maze->MapToScreenPosition(best->position);

		for (auto& dir : maze->directions) {
			const vec2i pos = maze->ScreenToMapPosition(dir + screenPosition);
			const int index = maze->GetMapIndex(pos);

			if (maze->GetMapData(pos) <= 0 || !vision[index] || unitIndexes[index]) continue;

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
					//opened.insert(opened.begin() + i, neighbour);
					opened.push_back(neighbour);
				}
			}
		}
	}

	std::vector<vec2i> path;

	auto last = closed.back();
	while (last) {
		if (!last->previous)
			break;

		for (int i = 0; i < maze->GetMapData(maze->GetMapIndex(last->position)); ++i) {
			path.insert(path.begin(), last->position);
		}
		last = last->previous;
	}

	return path;
}

unsigned Team::Move(const float& dt, EntityManager * const entities) {
	unsigned steps = 0;

	for (auto& unit : units) {
		if (!unit->path.empty()) {
			const vec2f position = maze->MapToScreenPosition(unit->path.front());
			entities->GetComponent<Animation>(unit->transform->entity)->Animate(
				AnimationBase(false, dt * 0.9f),
				unit->transform->translation.xy,
				position	
			);
			Scan(unit, position);
			unit->path.erase(unit->path.begin());
			++steps;
		}
	}

	return steps;
}

void Team::Scan(Unit * const unit, const vec2f& position) {
	bool deadend = true;

	unit->vision.clear();

	const auto outerPositions = maze->GetTilesAtRange(unit->viewRange, position);
	for (auto& pos : outerPositions) {
		const auto dir = (pos - position) / unit->viewRange;

		for (float i = 0.f; i <= unit->viewRange; ++i) {
			const auto target = maze->ScreenToMapPosition(position + dir * i);
			const int index = maze->GetMapIndex(target);

			if (index < 0) break;

			unit->vision.push_back(index);
			vision[index] = true;
			if (GetOpponentUnit(index))
				unit->visibleUnits[index] = unit;

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

void Team::MakeMove(EntityManager * const entities) {
	//entities->GetComponent<StateContainer>(ai)->queuedState = targetState;
	entities->GetComponent<StateContainer>(ai)->queuedState = "IDLE";
}
