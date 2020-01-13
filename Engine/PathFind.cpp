//#include "PathFind.h"
//
//#include "Transform.h"
//#include "Render.h"
//
//#include <Events/EventsManager.h>
//#include "PathFind.h"
//
//DNode::DNode()
//	: previous(nullptr)
//	, g(0.f)
//	, h(0.f)
//	, position(-1.f) {}
//
//float DNode::f() const {
//	return g + h;
//}
//
//bool DNode::operator<(const DNode & rhs) const {
//	return position.x < rhs.position.x || position.y < rhs.position.y;
//}
//
//PathFind::PathFind() {
//
//}
//
//void PathFind::Initialize() {}
//
//void PathFind::Explore(EntityManager* entities) {
//	auto transform = entities->GetComponent<Transform>(entity);
//
//	if (queuedPositions.empty()) {
//		const auto position = transform->GetWorldTranslation();
//		queuedPositions.push(maze->ScreenToMapPosition(position.xy));
//	}
//
//	transform->translation = vec3f(maze->MapToScreenPosition(queuedPositions.top()), 0.1f);
//	Scan(queuedPositions.top());
//}
//
//bool PathFind::IsDoneExploring() const {
//	return queuedPositions.empty();
//}
//
//void PathFind::Goto(const vec2i & target) {
//	auto transform = entities->GetComponent<Transform>(entity);
//
//	const auto position = GetMapPosition();
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
//		for (auto& dir : directions) {
//			const vec2i pos = dir + best->position;
//
//			if (maze->IsBlocked(pos)) continue;
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
//			DNode* neighbour = new DNode;
//			neighbour->g = best->g + 1;
//			neighbour->h = Math::LengthSquared<float>(pos - target);
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
//					opened.push_back(neighbour);
//				}
//			}
//		}
//	}
//
//	Events::EventsManager::GetInstance()->Trigger("DRAW_PATH", new Events::AnyType<std::vector<vec2i>>(GetPath(closed.back())));
//}
//
//void PathFind::SetMaze(Maze * const _maze) {
//	maze = _maze;
//	vision.resize(maze->GetSize() * maze->GetSize(), FOG);
//}
//
//const std::vector<unsigned>& PathFind::GetVision() const {
//	return vision;
//}
//
//void PathFind::Scan(const vec2i & curr) {
//	bool deadend = true;
//
//	for (auto& dir : directions) {
//		const auto newPosition = curr + dir;
//		const int index = maze->GetMapIndex(newPosition);
//		//Scan(curr + dir);
//		if (index < 0 || vision[index] != FOG) continue;
//
//		const bool isBlocked = maze->IsBlocked(index);
//		if (isBlocked) {
//			vision[index] = WALL;
//			continue;
//		}
//
//		vision[index] = PATH;
//		queuedPositions.push(newPosition);
//		deadend = false;
//	}
//
//	if (deadend)
//		queuedPositions.pop();
//}
//
//std::vector<vec2i> PathFind::GetPath(DNode * end) {
//	std::vector<vec2i> path;
//	while (end->previous) {
//		path.insert(path.begin(), end->position);
//		end = end->previous;
//	}
//
//	return path;
//}
//
//
