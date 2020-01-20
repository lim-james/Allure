#include "Mouse.h"

#include "Transform.h"
#include "Render.h"

#include <Events/EventsManager.h>

Mouse::Mouse(Transform * const transform) : transform(transform) {
	directions[0] = vec2i(1, 0);
	directions[1] = vec2i(0, 1);
	directions[2] = vec2i(-1, 0);
	directions[3] = vec2i(0, -1);
}

void Mouse::Explore() {
	if (queuedPositions.empty()) {
		auto position = maze->ScreenToMapPosition(transform->translation.xy);
		queuedPositions.push(position);
	}

	transform->translation.Set(maze->MapToScreenPosition(queuedPositions.top()), 0.1f);
	Scan(queuedPositions.top());
}

bool Mouse::IsDoneExploring() const {
	return queuedPositions.empty();
}

void Mouse::Goto(const vec2i & target) {
	const auto position = maze->ScreenToMapPosition(transform->GetWorldTranslation().xy);

	std::vector<DNode*> opened;
	std::vector<DNode*> closed;

	DNode* current = new DNode;
	current->g = 0;
	current->h = Math::LengthSquared<float>(target - position);
	current->position = position;

	opened.push_back(current);

	while (!opened.empty()) {
		DNode* best = opened.front();
		opened.erase(opened.begin());
		closed.push_back(best);

		if (best->position == target)
			break;

		for (auto& dir : directions) {
			const vec2i pos = dir + best->position;

			if (maze->IsBlocked(pos)) continue;

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

			DNode* neighbour = new DNode;
			neighbour->g = best->g + 1;
			neighbour->h = Math::LengthSquared<float>(pos - target);
			neighbour->position = pos;
			neighbour->previous = best;

			if (i != opened.size()) {
				if (opened[i]->g > neighbour->g) {
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
					opened.push_back(neighbour);
				}
			}
		}
	}

	Events::EventsManager::GetInstance()->Trigger("DRAW_PATH", new Events::AnyType<std::vector<vec2i>>(GetPath(closed.back())));
}

void Mouse::SetMaze(Maze * const _maze) {
	maze = _maze;
	vision.resize(maze->GetSize() * maze->GetSize(), FOG);
}

const std::vector<unsigned>& Mouse::GetVision() const {
	return vision;
}

void Mouse::Scan(const vec2i & curr) {
	bool deadend = true;

	for (auto& dir : directions) {
		const auto newPosition = curr + dir;
		const int index = maze->GetMapIndex(newPosition);
		//Scan(curr + dir);
		if (index < 0 || vision[index] != FOG) continue;

		const bool isBlocked = maze->IsBlocked(index);
		if (isBlocked) {
			vision[index] = WALL;
			continue;
		}

		vision[index] = PATH;
		queuedPositions.push(newPosition);
		deadend = false;
	}

	if (deadend)
		queuedPositions.pop();
}

std::vector<vec2i> Mouse::GetPath(DNode * end) {
	std::vector<vec2i> path;
	while (end->previous) {
		path.insert(path.begin(), end->position);
		end = end->previous;
	}

	return path;
}


