#ifndef MOUSE_H
#define MOUSE_H

#include "Maze.h"
#include "Transform.h"

#include <Math/Vectors.hpp>

#include <vector>
#include <stack>

struct DNode {

	DNode* previous;
	float g, h;
	vec2i position;

	DNode();

	float f() const;

	bool operator<(const DNode& rhs) const;

};

class Mouse {

	Transform * const transform;

	vec2i directions[4];

	std::stack<vec2i> queuedPositions;

	Maze* maze;
	std::vector<unsigned> vision;

public:

	Mouse(Transform * const transform);

	void Explore();
	bool IsDoneExploring() const;

	void Goto(const vec2i& target);

	void SetMaze(Maze* const _maze);

	const std::vector<unsigned>& GetVision() const;

private:

	void Scan(const vec2i& curr);

	std::vector<vec2i> GetPath(DNode* end);

};

#endif