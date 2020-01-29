#include "HexMaze.h"

HexMaze::HexMaze(const unsigned & size, const int & defaultTile) 
	: HexGrid(size, defaultTile) { }

void HexMaze::Generate(const unsigned& seed, const vec2i & start, const float & wallLoad) {
	std::fill(grid.begin(), grid.end(), defaultTile);
	srand(seed);

	const unsigned total = size * size;
	dir = 1;
	stream = MapToScreenPosition(start);
	const unsigned startIndex = GetMapIndex(start);

	//grid[startIndex] = WATER;

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

	for (int x = 0; x < 3; ++x) {
		for (int y = 0; y < 3; ++y) {
			int index = GetMapIndex(vec2i(x, y));
			if (grid[index] == WALL)
				grid[index] = MUD;

			index = GetMapIndex(vec2i(size - x - 1, size - y - 1));
			if (grid[index] == WALL)
				grid[index] = MUD;
		}
	}

}

void HexMaze::Update() {
	//stream += directions[1];
	//const int index = GetMapIndex(ScreenToMapPosition(stream));
	//if (index < 0) return;

	//for (float r = 0.f; r < 2.f; ++r) {
	//	for (auto i : GetTileIndexesAtRange(r, stream)) {
	//		if (i < 0) continue;
	//		grid[i] = WATER;
	//	}
	//}
}

bool HexMaze::IsBlocked(const vec2i & position) const {
	const int index = GetMapIndex(position);
	return index < 0 || grid[index] == WALL;
}

vec4f HexMaze::GetColour(const int & type) const {
	switch (type) {
	case WALL:
		return vec4f(0.2f, 0.2f, 0.2f, 0.3f);
	case FOG:
		return vec4f(0.f);
	case PATH:
		return vec4f(0.4f, 0.5f, 0.2f, 0.3f);
	case WATER:
		return vec4f(0.6f, 0.8f, 0.8f, 0.3f);
	case MUD:
		return vec4f(0.6f, 0.2f, 0.2f, 0.3f);
	default:
		return vec4f(0.f);
	}
}
