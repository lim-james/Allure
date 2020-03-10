#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <Math/Vectors.h>

struct Keyframe {

	unsigned texture;
	vec4f uvRect;

	float duration;

	Keyframe();

	void SetTilemapSize(const int& width, const int& height);
	void SetCellRect(const int& x, const int& y, const int& width, const int& height);

private:

	vec2f tilemapUnit;
	vec4f cellRect;

};

#endif
