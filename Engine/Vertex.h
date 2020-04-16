#ifndef VERTEX_H
#define VERTEX_H

#include <Math/Vectors.h>

struct Vertex {

	vec3f position;
	vec3f normal;
	vec2f texCoords;

	bool operator==(Vertex const& rhs) const;

};

#endif