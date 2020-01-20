#include "DNode.h"

DNode::DNode()
	: previous(nullptr)
	, g(0.f)
	, h(0.f)
	, position(-1.f) {}

float DNode::f() const {
	return g + h;
}

bool DNode::operator<(const DNode & rhs) const {
	return position.x < rhs.position.x || position.y < rhs.position.y;
}

