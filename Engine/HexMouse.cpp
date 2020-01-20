//#include "HexMouse.h"
//
//#include "Transform.h"
//#include "Render.h"
//
//#include <Events/EventsManager.h>
//
//HexMouse::HexMouse(Transform * const transform) : transform(transform) {}
//
//void HexMouse::Explore() {
//	if (queuedPositions.empty()) {
//		auto position = maze->ScreenToMapPosition(transform->translation.xy);
//		queuedPositions.push(position);
//	}
//
//	transform->translation.Set(maze->MapToScreenPosition(queuedPositions.top()), 0.1f);
//	Scan(queuedPositions.top());
//}
//
//bool HexMouse::IsDoneExploring() const {
//	return queuedPositions.empty();
//}
//
//void HexMouse::Goto(const vec2i & target) {
//	const auto position = maze->ScreenToMapPosition(transform->GetWorldTranslation().xy);
//
//	std::vector<DNode*> opened;
//	std::vector<DNode*> closed;
//
//	DNode* current = new DNode;
//	current->g = 0;
//	current->h = Math::LengthSquared<float>(target - position);
//	current->position = position;
//
//	opened.push_back(current);
//
//	while (!opened.empty()) {
//		DNode* best = opened.front();
//		opened.erase(opened.begin());
//		closed.push_back(best);
//
//		if (best->position == target)
//			break;
//
//		const vec2f screenPosition = maze->MapToScreenPosition(best->position);
//
//		for (auto& dir : directions) {
//			const vec2i pos = maze->ScreenToMapPosition(dir + screenPosition);
//
//			if (maze->GetMapData(pos)) continue;
//
//			bool found = false;
//			for (auto& node : closed) {
//				if (node->position == pos) {
//					found = true;
//					break;
//				}
//			}
//
//			if (found) continue;
//
//			unsigned i = 0;
//			for (; i < opened.size(); ++i) {
//				if (opened[i]->position == pos) {
//					break;
//				}
//			}
//
//			const float weight = static_cast<float>(maze->GetTileType(maze->GetMapIndex(pos)));
//
//			DNode* neighbour = new DNode;
//			neighbour->g = best->g + 1;
//			neighbour->h = Math::LengthSquared<float>(pos - target) * weight;
//			neighbour->position = pos;
//			neighbour->previous = best;
//
//			if (i != opened.size()) {
//				if (opened[i]->g > neighbour->g) {
//					delete opened[i];
//					opened[i] = neighbour;
//				} else {
//					delete neighbour;
//				}
//			} else {
//				i = 0;
//				for (; i < opened.size(); ++i) {
//					if (opened[i]->f() > neighbour->f()) {
//						opened.insert(opened.begin() + i, neighbour);
//						break;
//					}
//				}
//
//				if (i == opened.size()) {
//					opened.insert(opened.begin() + i, neighbour);
//					opened.push_back(neighbour);
//				}
//			}
//		}
//	}
//
//	Events::EventsManager::GetInstance()->Trigger("DRAW_PATH", new Events::AnyType<std::vector<vec2i>>(GetPath(closed.back())));
//}
//
//void HexMouse::SetMaze(HexMaze * const _maze) {
//	maze = _maze;
//	vision.resize(maze->GetSize() * maze->GetSize(), FOG);
//	visited.resize(maze->GetSize() * maze->GetSize(), false);
//}
//
//Transform * const HexMouse::GetTransform() const {
//	return transform;
//}
//
//const std::vector<unsigned>& HexMouse::GetVision() const {
//	return vision;
//}
//
//void HexMouse::Scan(const vec2i & curr) {
//	bool deadend = true;
//
//	const vec2f screenPosition = maze->MapToScreenPosition(curr);
//
//	for (float r = 3.f; r > 2.f; --r) {
//		for (auto& position : maze->GetTilesAtRange(r, screenPosition)) {
//			if (position < 0) continue;
//			vision[position] = maze->GetTileType(position);
//		}
//	}
//
//	for (auto& dir : directions) {
//		const auto newPosition = maze->ScreenToMapPosition(screenPosition + dir);
//		const int index = maze->GetMapIndex(newPosition);
//		//Scan(curr + dir);
//		if (index < 0 || visited[index]) continue;
//
//		visited[index] = true;
//
//		const bool isBlocked = maze->IsBlocked(index);
//		if (isBlocked) {
//			vision[index] = WALL;
//			continue;
//		}
//
//		vision[index] = maze->GetTileType(index);
//		queuedPositions.push(newPosition);
//		deadend = false;
//	}
//
//	if (deadend)
//		queuedPositions.pop();
//}
//
//std::vector<vec2i> HexMouse::GetPath(DNode * end) {
//	std::vector<vec2i> path;
//
//	while (end->previous) {
//		path.insert(path.begin(), end->position);
//		end = end->previous;
//	}
//
//	return path;
//}
//
//
