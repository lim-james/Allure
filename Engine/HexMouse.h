//#ifndef HEX_MOUSE_H
//#define HEX_MOUSE_H
//
//#include "HexMaze.h"
//#include "HexGrid.h"
//#include "Transform.h"
//
//#include <Math/Vectors.hpp>
//
//#include <vector>
//#include <stack>
//
//#include "Mouse.h"
//
//class HexMouse {
//
//	Transform * const transform;
//
//	std::stack<vec2i> queuedPositions;
//
//	HexMaze* maze;
//
//	HexGrid grid;
//	std::vector<bool> visited;
//
//public:
//
//	HexMouse(Transform * const transform);
//
//	void Explore();
//	bool IsDoneExploring() const;
//
//	void Goto(const vec2i& target);
//
//	void SetMaze(HexMaze* const _maze);
//
//	Transform * const GetTransform() const;
//
//	const std::vector<unsigned>& GetVision() const;
//
//private:
//
//	void Scan(const vec2i& curr);
//
//	std::vector<vec2i> GetPath(DNode* end);
//
//};
//
//#endif