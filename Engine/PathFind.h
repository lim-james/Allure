//#ifndef MOUSE_H
//#define MOUSE_H
//
//#include "Component.h"
//
//#include "Maze.h"
//
//#include <Math/Vectors.hpp>
//
//#include <vector>
//#include <stack>
//
//struct DNode {
//
//	DNode* previous;
//	float g, h;
//	vec2i position;
//
//	DNode();
//
//	float f() const;
//
//	bool operator<(const DNode& rhs) const;
//
//};
//
//struct PathFind : Component {
//
//	void Initialize() override;
//
//	void Explore();
//	bool IsDoneExploring() const;
//
//	void Goto(const vec2i& target);
//
//	void SetMaze(Maze* const _maze);
//
//	const std::vector<unsigned>& GetVision() const;
//
//	Maze* maze;
//	std::stack<vec2i> queuedPositions;
//	std::vector<unsigned> vision;
//
//};
//
//#endif