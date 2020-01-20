#ifndef HEX_GRID_H
#define HEX_GRID_H

#include <Math/Vectors.hpp>

#include <vector>

class HexGrid {

protected:

	vec2f directions[6];

	unsigned size;
	std::vector<int> grid;

public:

	HexGrid(const unsigned& size, const int& defaultTile);

	const unsigned& GetSize() const;

	bool IsValid(const vec2i& position) const;
	bool IsValid(const int& index) const;

	int GetMapData(const vec2i& position) const;
	int GetMapData(const int& x, const int& y) const;
	int GetMapData(const int& index) const;

	void SetMapData(const vec2i& position, const int& tile);
	void SetMapData(const int& x, const int& y, const int& tile);
	void SetMapData(const int& index, const int& tile);

	int GetMapIndex(const vec2i& position) const;
	int GetMapIndex(const int& x, const int& y) const;

	vec2i GetMapPosition(const int& index) const;

	vec2i ScreenToMapPosition(const vec2f& position) const;
	vec2f MapToScreenPosition(const vec2i& position) const;

	std::vector<vec2f> GetTilesAtRange(const float& r, const vec2f& position) const;
	std::vector<int> GetTileIndexesAtRange(const float& r, const vec2f& position) const;

};

#endif
