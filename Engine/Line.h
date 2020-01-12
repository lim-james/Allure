#ifndef LINE_H
#define LINE_H

#include <Math/Vectors.hpp>

struct Line {
	vec2f offset;
	vec2f length;
	vec4f tint;

	void Set(const vec2f& start, const vec2f& end);
};

#endif
