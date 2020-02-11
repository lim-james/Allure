#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"
#include "Edge.h"

#include <vector>
#include <map>
#include <stack>
#include <functional>

struct GNode {
	GNode* previous;
	Node* node;
	Edge* edge;

	float g, h;

	float f() const;
};

class Graph {

	std::vector<Node*> nodes;
	std::vector<Edge*> edges;

	std::vector<Edge*> path;

	std::function<void(Node*)> createNodeHandler;
	std::function<void(Edge*)> createEdgeHandler;

public:

	~Graph();

	void Create(const std::string& filepath);
	void Update();

	Node * const CreateNode(const float& x, const float& y, const float& z);
	Node * const CreateNode(const vec3f& position);

	Node * const GetNearestNode(const vec3f& position) const;

	Edge * const CreateEdge(const unsigned& from, const unsigned& to, const float& weight = 1.f);

	void DrawEdges();

	template<typename Context>
	void BindCreateNodeHandler(void(Context::*callback)(Node*), Context* context);

	template<typename Context>
	void BindCreateEdgeHandler(void(Context::*callback)(Edge*), Context* context);

	void ClearPath();
	void PathFind(Node * const start, Node * const end);

};

template<typename Context>
void Graph::BindCreateNodeHandler(void(Context::* callback)(Node*), Context * context) {
	createNodeHandler = std::bind(callback, context, std::placeholders::_1);
}

template<typename Context>
void Graph::BindCreateEdgeHandler(void(Context::* callback)(Edge*), Context * context) {
	createEdgeHandler = std::bind(callback, context, std::placeholders::_1);
}

#endif

