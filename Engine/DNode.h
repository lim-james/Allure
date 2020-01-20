#ifndef D_NODE_H
#define D_NODE_H

#include <Math/Vectors.hpp>

struct DNode {

	DNode* previous;
	float g, h;
	vec2i position;

	DNode();

	float f() const;

	bool operator<(const DNode& rhs) const;

};

#endif
