#ifndef LINE_H
#define LINE_H

#include <Math/Vectors.h>

struct Line {
	vec2f offset;
	vec2f length;
	vec4f tint;

	void Set(vec2f const& start, vec2f const& end);
};

#endif
