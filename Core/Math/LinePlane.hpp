#ifndef LINE_PLANE_HPP
#define LINE_PLANE_HPP

#include "Vectors.h"

namespace Math {

	template<typename T>
	struct Line {
		Math::vec3<T> r0;
		Math::vec3<T> d;
	};

	template<typename T>
	Math::vec3<T> GetPoint(Line<T> const& line, T const& t) {
		return line.r0 + line.d * t;
	}

	template<typename T>
	Math::vec3<T> GetPoint(T const& t, Line<T> const& line) {
		return line.r0 + line.d * t;
	}

	template<typename T>
	struct Plane {
		Math::vec3<T> r0;
		Math::vec3<T> n;
	};

	template<typename T>
	T Intersection(Line<T> const& line, Plane<T> const& plane) {
		return (Math::dot(plane.n, plane.r0) - Math::dot(plane.n, line.r0)) / Math::dot(plane.n, line.d);
	}

	template<typename T>
	T Intersection(Plane<T> const& plane, Line<T> const& line) {
		return (Math::dot(plane.n, plane.r0) - Math::dot(plane.n, line.r0)) / Math::dot(plane.n, line.d);
	}

}

typedef Math::Line<float> LineF;
typedef Math::Plane<float> PlaneF;

#endif