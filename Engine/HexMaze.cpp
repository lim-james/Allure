#include "HexMaze.h"

void HexMaze::Generate(const unsigned& seed, const unsigned & _size, const vec2i & start, const float & wallLoad) {
	grid.clear();
	size = _size;
	if (size == 0)
		return;

	const unsigned total = size * size;
	grid.resize(total, PATH);
	srand(seed);

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

const unsigned & HexMaze::GetSize() const {
	return size;
}

bool HexMaze::IsBlocked(const vec2i & position) const {
	const int index = GetMapIndex(position);
	return index < 0 || grid[index] == WALL;
}

bool HexMaze::IsBlocked(const int& index) const {
	return index < 0 || grid[index] == WALL;
}

int HexMaze::GetTileType(const int & index) const {
	return grid[index];
}

int HexMaze::GetMapData(const vec2i & position) const {
	const int index = GetMapIndex(position);
	if (index < 0) return WALL;
	return grid[index];
}

int HexMaze::GetMapData(const int & x, const int & y) const {
	const int index = GetMapIndex(x, y);
	if (index < 0) return WALL;
	return grid[index];
}

int HexMaze::GetMapData(const int & index) const {
	return grid[index];
}

int HexMaze::GetMapIndex(const vec2i & position) const {
	const int i = static_cast<int>(size);

	if (position.x < 0 || position.x >= i ||
		position.y < 0 || position.y >= i)
		return -1;

	return position.x + position.y * i;
}

int HexMaze::GetMapIndex(const int & x, const int & y) const {
	const int i = static_cast<int>(size);

	if (x < 0 || x >= i ||
		y < 0 || y >= i)
		return -1;

	return x + y * i;
}

vec2i HexMaze::GetMapPosition(const int & index) const {
	const int i = static_cast<int>(size);
	return vec2i(index % i, index / i);
}

vec2i HexMaze::ScreenToMapPosition(const vec2f & position) const {
	const int halfSize = static_cast<int>(size) * 0.5f;

	vec2f result = position;
	result.x = round((result.x - 0.5f) / 0.75f);
	result.y = round((result.y - static_cast<int>(abs(result.x)) % 2 * 0.433f) / 0.866f);

	return vec2i(
		static_cast<int>(result.x) + halfSize,
		static_cast<int>(result.y) + halfSize
	);
}

vec2f HexMaze::MapToScreenPosition(const vec2i & position) const {
	const float halfSize = static_cast<float>(size) * 0.5f;

	vec2f result(
		static_cast<float>(position.x) - halfSize,
		static_cast<float>(position.y) - halfSize
	);

	result.x = result.x * 0.75f + 0.5f;
	result.y = result.y * 0.866f + static_cast<int>(abs(position.x)) % 2 * 0.433f;

	return result;
}
