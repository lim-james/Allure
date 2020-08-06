#include "TilemapResource.h"

TilemapTexture::TilemapTexture()
	: dimensions(1)
	, unit(1.f) {}

void TilemapTexture::SetDimensions(vec2u const & dim) {
	dimensions = dim;
	unit = 1.f / vec2f(dim);
}

vec2u TilemapTexture::GetDimensions() const {
	return dimensions;
}

vec2f TilemapTexture::GetUnit() const {
	return unit;
}

vec2f TilemapTexture::GetOffset(unsigned const & index) const {
	const unsigned x = index % dimensions.x;
	const unsigned y = index / dimensions.x;
	return vec2f(static_cast<float>(x), static_cast<float>(y));
}

TilemapPalette::TilemapPalette() {}

void TilemapPalette::AddTexture(TilemapTexture const & tex) {
	textureIndex[tex.texture].push_back(textures.size());
	textures.push_back(tex);
}

TilemapTexture TilemapPalette::GetTexture(unsigned const & index) const {
	return textures[index];
}

std::vector<TilemapTexture> TilemapPalette::GetTextures() const {
	return textures;
}

std::vector<unsigned> TilemapPalette::GetTextureIndexes(unsigned const & tex) const {
	return textureIndex.at(tex);
}

