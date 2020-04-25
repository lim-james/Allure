#ifndef PLANE_H
#define PLANE_H

#include <Math/Vectors.h>

struct Plane {
	vec3f origin, normal;

	Plane(vec3f const& origin = 0.0f, vec3f const& normal = vec3f(0.f, 1.f, 0.f))
		: origin(origin)
		, normal(normal) {}
};

#endif
