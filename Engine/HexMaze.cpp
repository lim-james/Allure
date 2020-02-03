#include "HexMaze.h"

#include <Math/Random.hpp>

#include <stack>
#include <time.h>

HexMaze::HexMaze(const unsigned & size, const int & defaultTile) 
	: HexGrid(size, defaultTile) 
	, et(0.f) { }

void HexMaze::Generate(const vec2i & start, const float & wallLoad) {
	std::fill(grid.begin(), grid.end(), WALL);
	srand(time(nullptr));

	const unsigned total = size * size;
	dir = 1;
	stream = MapToScreenPosition(start);
	const unsigned startIndex = GetMapIndex(start);

	int dir = Math::RandMinMax(0, 6);

	std::stack<vec2f> anchors;
	anchors.push(stream);

	auto origin = stream;

	int index = GetMapIndex(ScreenToMapPosition(origin));

	const vec2f bottomLeft = MapToScreenPosition(vec2i(0));

	while (index >= 0) {
		const auto dir = bottomLeft - stream;
		const auto e = vec2f(Math::RandMinMax(-1.f, 1.f), Math::RandMinMax(-1.f, 1.f));
		stream += Math::Normalized(dir + e);
		index = GetMapIndex(ScreenToMapPosition(stream));
		grid[index] = PATH;
		for (auto& tile : GetTileIndexesAtRange(1.f, stream)) {
			grid[tile] = PATH;
		}
	}

	stream = origin;
	index = GetMapIndex(ScreenToMapPosition(origin));

	while (index >= 0) {
		const auto dir = bottomLeft - stream;
		const auto e = vec2f(Math::RandMinMax(-1.f, 1.f), Math::RandMinMax(-1.f, 1.f));
		stream -= Math::Normalized(dir + e);
		index = GetMapIndex(ScreenToMapPosition(stream));
		grid[index] = PATH;
		for (auto& tile : GetTileIndexesAtRange(1.f, stream)) {
			grid[tile] = PATH;
		}
	}

	//while (anchors.size()) {
	//	int streamIndex = GetMapIndex(ScreenToMapPosition(stream));
	//	if (streamIndex < 0 || grid[streamIndex] > 0) {
	//		anchors.pop();
	//		if (anchors.empty()) break;
	//		stream = anchors.top();
	//		streamIndex = GetMapIndex(ScreenToMapPosition(stream));
	//	}

	//	grid[streamIndex] = PATH;
	//	for (auto& tile : GetTileIndexesAtRange(1.f, stream)) {
	//		grid[tile] = PATH;
	//	}
	//	
	//	stream += directions[dir] * 2.f;

	//	if (Math::RandMinMax(0, 6) == 0) {
	//		dir += Math::RandSign();
	//		if (dir < 0) dir = 5;
	//		else if (dir == 6) dir = 0;
	//		anchors.push(stream);
	//	} 	
	//}

	//for (unsigned i = 0; i < static_cast<unsigned>(total * wallLoad);) {
	//	unsigned chosen = rand() % total;
	//	if (chosen == startIndex)
	//		continue;

	//	if (grid[chosen] == PATH) {
	//		int r = rand() % 10;
	//		if (r > 5) {
	//			grid[chosen] = WALL;
	//		} else if (r > 3) {
	//			grid[chosen] = MUD;
	//		} else {
	//			grid[chosen] = WATER;
	//		}
	//		++i;
	//	}
	//}

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

bool HexMaze::Update(const float& dt) {
	et += dt;
	if (flowing) {
		int index = GetMapIndex(ScreenToMapPosition(stream));

		if (index >= 0) {
			grid[index] = WATER;
			for (auto i : GetTileIndexesAtRange(1.f, stream)) {
				grid[i] = WATER;
			}
			stream += dir;
			return true;
		} else {
			flowing = false;
			return false;
		}
	} else {
		if (et - lastWave > 3.f) {
			for (auto& tile : grid) {
				if (tile == WATER) {
					tile = Math::RandValue() > 0.6f ? MUD : PATH;
				} else if (tile == MUD) {
					tile = WALL;
				}
			}

			lastWave = et;
			stream = MapToScreenPosition(vec2i(0, Math::RandMinMax(2, (int)size - 3)));
			dir = Math::Normalized(vec2f(1.f, Math::RandMinMax(-0.5f, 0.5f)));
			const int index = GetMapIndex(ScreenToMapPosition(stream));
			grid[index] = WATER;
			flowing = true;
			return true;
		}
	}

	return false;
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
