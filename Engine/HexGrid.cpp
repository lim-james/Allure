#include "HexGrid.h"

HexGrid::HexGrid(const unsigned& size, const int& defaultTile) : size(size) {
	directions[0] = vec2f(0.f,  0.866f);
	directions[1] = vec2f( 0.75f,  0.433f);
	directions[2] = vec2f( 0.75f, -0.433f);
	directions[3] = vec2f(0.f, -0.866f);
	directions[4] = vec2f(-0.75f, -0.433f);
	directions[5] = vec2f(-0.75f,  0.433f);

	const unsigned total = size * size;
	grid.resize(total, defaultTile);
}

const unsigned & HexGrid::GetSize() const {
	return size;
}

bool HexGrid::IsValid(const vec2i & position) const {
	const int index = GetMapIndex(position);
	return index < 0;
}

bool HexGrid::IsValid(const int& index) const {
	return index < 0;
}

int HexGrid::GetMapData(const vec2i & position) const {
	const int index = GetMapIndex(position);
	if (index < 0) return -1;
	return grid[index];
}

int HexGrid::GetMapData(const int & x, const int & y) const {
	const int index = GetMapIndex(x, y);
	if (index < 0) return -1;
	return grid[index];
}

int HexGrid::GetMapData(const int & index) const {
	return grid[index];
}

void HexGrid::SetMapData(const vec2i & position, const int& tile) {
	const int index = GetMapIndex(position);
	if (index < 0) return;
	grid[index] = tile;
}

void HexGrid::SetMapData(const int & x, const int & y, const int& tile) {
	const int index = GetMapIndex(x, y);
	if (index < 0) return;
	grid[index] = tile;
}

void HexGrid::SetMapData(const int & index, const int& tile) {
	if (index < 0) return;
	grid[index] = tile;
}

int HexGrid::GetMapIndex(const vec2i & position) const {
	const int i = static_cast<int>(size);

	if (position.x < 0 || position.x >= i ||
		position.y < 0 || position.y >= i)
		return -1;

	return position.x + position.y * i;
}

int HexGrid::GetMapIndex(const int & x, const int & y) const {
	const int i = static_cast<int>(size);

	if (x < 0 || x >= i ||
		y < 0 || y >= i)
		return -1;

	return x + y * i;
}

vec2i HexGrid::GetMapPosition(const int & index) const {
	const int i = static_cast<int>(size);
	return vec2i(index % i, index / i);
}

vec2i HexGrid::ScreenToMapPosition(const vec2f & position) const {
	const int halfSize = static_cast<int>(size) * 0.5f;

	vec2f result = position;
	result.x = round((result.x - 0.5f) / 0.75f);
	result.y = round((result.y - static_cast<int>(abs(result.x)) % 2 * 0.433f) / 0.866f);

	return vec2i(
		static_cast<int>(result.x) + halfSize,
		static_cast<int>(result.y) + halfSize
	);
}

vec2f HexGrid::MapToScreenPosition(const vec2i & position) const {
	const float halfSize = static_cast<float>(size) * 0.5f;

	vec2f result(
		static_cast<float>(position.x) - halfSize,
		static_cast<float>(position.y) - halfSize
	);

	result.x = result.x * 0.75f + 0.5f;
	result.y = result.y * 0.866f + static_cast<int>(abs(position.x)) % 2 * 0.433f;

	return result;
}

std::vector<vec2f> HexGrid::GetTilesAtRange(const float & r, const vec2f & position) const {
	if (r == 0) {
		return { position };
	} 

	std::vector<vec2f> corners;
	std::vector<vec2f> result;
	corners.push_back(r * directions[0] + position);
	result.push_back(corners.back());

	for (unsigned i = 1; i < 6; ++i) {
		const auto prev = corners.back();
		const auto curr = r * directions[i] + position;
		const auto dir = (curr - prev) / r;

		for (float d = 0.f; d < r; ++d) {
			result.push_back(prev + d * dir);
		}
		corners.push_back(curr);
	}

	const auto prev = corners.back();
	const auto curr = corners.front();
	const auto dir = (curr - prev) / r;

	for (float d = 0.f; d < r; ++d) {
		result.push_back(prev + d * dir);
	}

	return result;
}

std::vector<int> HexGrid::GetTileIndexesAtRange(const float& r, const vec2f& position) const {
	if (r == 0) {
		return { GetMapIndex(ScreenToMapPosition(position)) };
	} 

	std::vector<vec2f> corners;
	std::vector<int> result;
	corners.push_back(r * directions[0] + position);
	result.push_back(GetMapIndex(ScreenToMapPosition(corners.back())));

	for (unsigned i = 1; i < 6; ++i) {
		const auto prev = corners.back();
		const auto curr = r * directions[i] + position;
		const auto dir = (curr - prev) / r;

		for (float d = 0.f; d < r; ++d) {
			result.push_back(GetMapIndex(ScreenToMapPosition(prev + d * dir)));
		}
		corners.push_back(curr);
	}

	const auto prev = corners.back();
	const auto curr = corners.front();
	const auto dir = (curr - prev) / r;

	for (float d = 0.f; d < r; ++d) {
		auto pos = ScreenToMapPosition(prev + d * dir);
		result.push_back(GetMapIndex(pos));
	}

	return result;
}

std::vector<vec2i> HexGrid::GetPath(const vec2f & start, const vec2f & end) const {
	const auto position = ScreenToMapPosition(start);
	const auto target = ScreenToMapPosition(end);

	std::vector<DNode*> opened;
	std::vector<DNode*> closed;

	DNode* current = new DNode;
	current->g = 0;
	current->h = Math::LengthSquared<float>(target - position);
	current->position = position;
	current->previous = nullptr;

	opened.push_back(current);

	while (!opened.empty()) {
		DNode* best = opened.front();
		opened.erase(opened.begin());
		closed.push_back(best);

		if (best->position == target)
			break;

		const vec2f screenPosition = MapToScreenPosition(best->position);

		for (auto& dir : directions) {
			const vec2i pos = ScreenToMapPosition(dir + screenPosition);

			if (GetMapData(pos) <= 0) continue;

			bool found = false;
			for (auto& node : closed) {
				if (node->position == pos) {
					found = true;
					break;
				}
			}

			if (found) continue;

			unsigned i = 0;
			for (; i < opened.size(); ++i) {
				if (opened[i]->position == pos) {
					break;
				}
			}

			const float weight = static_cast<float>(GetMapData(GetMapIndex(pos)));

			DNode* neighbour = new DNode;
			neighbour->g = best->g + 1;
			neighbour->h = Math::LengthSquared<float>(pos - target) * weight;
			neighbour->position = pos;
			neighbour->previous = best;

			if (i != opened.size()) {
				if (opened[i]->f() > neighbour->f()) {
					delete opened[i];
					opened[i] = neighbour;
				} else {
					delete neighbour;
				}
			} else {
				i = 0;
				for (; i < opened.size(); ++i) {
					if (opened[i]->f() > neighbour->f()) {
						opened.insert(opened.begin() + i, neighbour);
						break;
					}
				}

				if (i == opened.size()) {
					opened.insert(opened.begin() + i, neighbour);
					opened.push_back(neighbour);
				}
			}
		}
	}

	std::vector<vec2i> path;

	auto last = closed.back();
	while (last) {
		for (unsigned i = 0; i < grid[GetMapIndex(last->position)]; ++i) {
			path.insert(path.begin(), last->position);
		}
		last = last->previous;
	}

	return path;
}
