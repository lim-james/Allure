#ifndef TILEMAP_RESOURCE_H
#define TILEMAP_RESOURCE_H

#include <Math/Vectors.h>
#include <vector>
#include <map>

struct TilemapTexture {
	unsigned texture;

	TilemapTexture();

	void SetDimensions(vec2u const& dim);
	vec2u GetDimensions() const;

	vec2f GetUnit() const;
	vec2f GetOffset(unsigned const& index) const;

private:

	vec2u dimensions;
	vec2f unit;
	
};

struct TilemapPalette {

	TilemapPalette();

	void AddTexture(TilemapTexture const& tex);
	TilemapTexture GetTexture(unsigned const& index) const;
	std::vector<TilemapTexture> GetTextures() const;

	std::vector<unsigned> GetTextureIndexes(unsigned const& tex) const;

private:

	std::vector<TilemapTexture> textures;
	std::map<unsigned, std::vector<unsigned>> textureIndex;

};

struct Tile {
	vec2f uvOffset;
	vec2f position;
};

struct TilemapLayout {
	std::vector<std::vector<Tile>> grids;
};

#endif
