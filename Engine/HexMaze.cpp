#include "HexMaze.h"

HexMaze::HexMaze(const unsigned & size, const int & defaultTile) 
	: HexGrid(size, defaultTile) { }

void HexMaze::Generate(const unsigned& seed, const vec2i & start, const float & wallLoad) {
	srand(seed);

	const unsigned total = size * size;
	const unsigned startIndex = GetMapIndex(start);

	for (unsigned i = 0; i < static_cast<unsigned>(total * wallLoad);) {
		unsigned chosen = rand() % total;
		if (chosen == startIndex)
			continue;

		if (grid[chosen] == PATH) {
			int r = rand() % 10;
			if (r > 5) {
				grid[chosen] = WALL;
			} else if (r > 3) {
				grid[chosen] = MUD;
			} else {
				grid[chosen] = WATER;
			}
			++i;
		}
	}
}

bool HexMaze::IsBlocked(const vec2i & position) const {
	const int index = GetMapIndex(position);
	return index < 0 || grid[index] == WALL;
}

vec4f HexMaze::GetColour(const int & type) const {
	switch (type) {
	case WALL:
		return vec4f(0.2f, 0.2f, 0.2f, 0.8f);
	case FOG:
		return vec4f(0.f);
	case PATH:
		return vec4f(0.4f, 0.5f, 0.2f, 0.8f);
	case WATER:
		return vec4f(0.6f, 0.8f, 0.8f, 0.8f);
	case MUD:
		return vec4f(0.6f, 0.2f, 0.2f, 0.8f);
	default:
		return vec4f(0.f);
	}
}
