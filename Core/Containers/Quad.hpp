#ifndef QUAD_H
#define QUAD_H

#include "../Math/Vectors.h"

#include <vector>

#define TOP_LEFT		0
#define TOP_RIGHT		1
#define BOTTOM_LEFT		2
#define BOTTOM_RIGHT	3
#define QUAD_COUNT		4

template<typename ContentType>
struct Quad {

	unsigned depth;

	Quad* children[QUAD_COUNT];
	Quad* parent;

	vec2f position, size;

	std::vector<ContentType> list;

	Quad(Quad<ContentType>* parent, vec2f const& position, vec2f const& size);

	template<typename ContentType>
	friend std::ostream& operator<<(std::ostream& os, Quad<ContentType> const& quad);

};

template<typename ContentType>
Quad<ContentType>::Quad(Quad<ContentType>* parent, vec2f const& position, vec2f const& size) 
	: parent(parent) 
	, position(position) 
	, size(size) {
	depth = parent ? parent->depth + 1 : 0;
	for (auto& child : children)
		child = nullptr;
}

template<typename ContentType>
std::ostream& operator<<(std::ostream& os, Quad<ContentType> const& quad) {
	for (unsigned i = 0; i < quad.depth; ++i)
		os << '\t';
	os << "Size: (" << quad.size << ")\n";
	for (unsigned i = 0; i < quad.depth; ++i)
		os << '\t';
	os << "Position: (" << quad.position << ")\n";
	for (unsigned i = 0; i < quad.depth; ++i)
		os << '\t';
	os << "Count: " << quad.list.size() << '\n';

	if (quad.children[0])
		for (auto& child : quad.children)
			os << *child;

	return os;
}

#endif

