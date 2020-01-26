#ifndef HEX_MAZE_H
#define HEX_MAZE_H

#include "HexGrid.h"

#define WALL -1
#define FOG   0
#define PATH  1
#define WATER 2
#define MUD	  3

class HexMaze : public HexGrid {

	int dir;
	vec2f stream;

public:

	HexMaze(const unsigned& size, const int& defaultTile);

	void Generate(const unsigned& seed, const vec2i & start, const float & wallLoad);
	void Update();
	
	bool IsBlocked(const vec2i& position) const;

	vec4f GetColour(const int& type) const;

};

#endif
