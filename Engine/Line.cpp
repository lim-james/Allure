#include "Line.h"

void Line::Set(const vec2f& start, const vec2f& end) {
	offset = start;
	length = end - start;
}
