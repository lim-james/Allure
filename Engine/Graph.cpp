#include "Graph.h"

#include "Line.h"

#include <Events/EventsManager.h>
#include <Loaders/LoadCSV.hpp>

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

Edge * const Graph::CreateEdge(const unsigned & from, const unsigned & to, const float & weight) {
	Edge* edge = new Edge;
	edge->from = nodes[from];
	edge->to = nodes[to];
	edge->weight = weight;

	edges.push_back(edge);

	if (createEdgeHandler)
		createEdgeHandler(edge);

	return edge;
}

void Graph::DrawEdges() const {
	for (auto& e : edges) {
		Line line;
		line.tint.Set(1.f);
		line.Set(e->from->position, e->to->position);
		Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));
	}
	
}

