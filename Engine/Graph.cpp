#include "Graph.h"

#include "Line.h"

#include <Events/EventsManager.h>
#include <Loaders/LoadCSV.hpp>

float GNode::f() const {
	return g + h;
}

Graph::~Graph() {
	for (auto& n : nodes)
		delete n;
	nodes.clear();

	for (auto& e : edges)
		delete e;
	edges.clear();
}

void Graph::Create(const std::string & filepath) {
	auto result = Load::CSV<float>(filepath);
	for (auto& row : result) {
		if (row.empty()) continue;

		if (row.front() == 0.f) {
			CreateNode(row[1], row[2], row[3]);
		} else if (row.front() == 1.f) {
			CreateEdge(row[1], row[2], row[3]);
		}
	}
}

void Graph::Update() {
	for (auto& edge : edges) {
		edge->weight = Math::LengthSquared(edge->from->position - edge->to->position);
	}
}

Node * const Graph::CreateNode(const float & x, const float & y, const float & z) {
	Node* node = new Node;
	node->id = nodes.size();
	node->position.Set(x, y, z);

	nodes.push_back(node);
	createNodeHandler(node);

	return node;
}

Node * const Graph::CreateNode(const vec3f & position) {
	Node* node = new Node;
	node->id = nodes.size();
	node->position = position;

	nodes.push_back(node);

	if (createNodeHandler)
		createNodeHandler(node);

	return node;
}

Node * const Graph::GetNearestNode(const vec3f & position) const {
	Node* closest = nodes.front();
	float weight = Math::LengthSquared(position - closest->position);

	for (unsigned i = 1; i < nodes.size(); ++i) {
		auto node = nodes[i];
		float curr = Math::LengthSquared(position - node->position);
		if (curr < weight) {
			closest = node;
			weight = curr;
		}
	}

	return closest;
}

Edge * const Graph::CreateEdge(const unsigned & from, const unsigned & to, const float & weight) {
	if (from == to) return nullptr;

	Edge* edge = new Edge;
	edge->from = nodes[from];
	edge->to = nodes[to];
	edge->weight = weight;

	nodes[from]->edges.push_back(edge);
	nodes[to]->edges.push_back(edge);

	edges.push_back(edge);

	if (createEdgeHandler)
		createEdgeHandler(edge);

	return edge;
}

void Graph::DrawEdges() {
	for (auto& e : edges) {
		bool inPath = false;
		for (auto& p : path) {
			if (e == p) {
				inPath = true;
				break;
			}
		}

		Line line;
		if (inPath) {
			line.tint.Set(1.f, 1.f, 0.f, 1.f);
		} else {
			line.tint.Set(0.5f);
		}

		line.Set(e->from->position, e->to->position);
		Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));
	}
	
}

void Graph::ClearPath() {
	path.clear();
}

void Graph::PathFind(Node * const start, Node * const end) {
	Update();

	std::vector<GNode*> opened;
	std::vector<GNode*> closed;

	GNode* current = new GNode;
	current->g = 0;
	current->h = Math::LengthSquared(end->position - start->position);
	current->previous = nullptr;
	current->node = start;
	current->edge = nullptr;

	opened.push_back(current);

	while (!opened.empty()) {
		GNode* best = opened.front();
		opened.erase(opened.begin());
		closed.push_back(best);

		if (best->node == end)
			break;

		for (auto& edge : best->node->edges) {
			auto target = edge->from;

			if (target == best->node)
				target = edge->to;

			bool found = false;
			for (auto& node : closed) {
				if (node->node == target) {
					found = true;
					break;
				} 			
			}

			if (found) continue;

			unsigned i = 0;
			for (; i < opened.size(); ++i) {
				if (opened[i]->node == target) {
					break;
				}
			}

			GNode* neighbour = new GNode;
			neighbour->previous = best;
			neighbour->g = best->g + edge->weight;
			neighbour->h = Math::LengthSquared(end->position - target->position);
			neighbour->node = target;
			neighbour->edge = edge;

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
					opened.push_back(neighbour);
				}
			}
		}
	}

	path.clear();

	auto last = closed.back();
	while (last) {
		if (last->edge) 
			path.insert(path.begin(), last->edge);
		last = last->previous;
	}

	path;
}
