#ifndef NODE_H
#define NODE_H

#include <Math/Vectors.hpp>

#include <vector>

struct Edge;

struct Node {

	unsigned id;
	unsigned entity;
	vec3f position;

	std::vector<Edge*> edges;

};

#endif
