#include "Line.h"

void Line::Set(vec2f const& start, vec2f const& end) {
	offset = start;
	length = end - start;
}
