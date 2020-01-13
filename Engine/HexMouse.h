#ifndef HEX_MOUSE_H
#define HEX_MOUSE_H

#include "HexMaze.h"
#include "Transform.h"

#include <Math/Vectors.hpp>

#include <vector>
#include <stack>

#include "Mouse.h"

class HexMouse {

	Transform * const transform;

	vec2f directions[6];

	std::stack<vec2i> queuedPositions;

	HexMaze* maze;
	std::vector<unsigned> vision;

public:

	HexMouse(Transform * const transform);

	void Explore();
	bool IsDoneExploring() const;

	void Goto(const vec2i& target);

	void SetMaze(HexMaze* const _maze);

	const std::vector<unsigned>& GetVision() const;

private:

	void Scan(const vec2i& curr);

	std::vector<vec2i> GetPath(DNode* end);

};

#endif