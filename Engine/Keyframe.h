#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <Math/Vectors.h>

struct Keyframe {

	unsigned texture;
	vec4f uvRect;

	float duration;

	Keyframe();

	void SetTilemapSize(int const& width, int const& height);
	void SetCellRect(int const& x, int const& y, int const& width, int const& height);

private:

	vec2f tilemapUnit;
	vec4f cellRect;

};

#endif
