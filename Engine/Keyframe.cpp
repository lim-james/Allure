#include "Keyframe.h"

Keyframe::Keyframe()
	: texture(0)
	, uvRect(vec2f(0.f), vec2f(1.f))
	, duration(0.f)
	, tilemapUnit(1.f)
	, cellRect(vec2f(0.f), vec2f(1.f)) {}

void Keyframe::SetTilemapSize(int const& width, int const& height) {
	tilemapUnit.x = 1.f / static_cast<float>(width);
	tilemapUnit.y = 1.f / static_cast<float>(height);

	uvRect = cellRect * vec4f(tilemapUnit, tilemapUnit);
}

void Keyframe::SetCellRect(int const& x, int const& y, int const& width, int const& height) {
	cellRect = vec4f(
		static_cast<float>(x),
		static_cast<float>(y),
		static_cast<float>(width),
		static_cast<float>(height)
	);

	uvRect = cellRect * vec4f(tilemapUnit, tilemapUnit);
}