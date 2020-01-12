#ifndef LINE_PLANE_HPP
#define LINE_PLANE_HPP

#include "Vectors.hpp"

namespace Math {

	template<typename T>
	struct Line {
		Math::vec3<T> r0;
		Math::vec3<T> d;
	};

	template<typename T>
	Math::vec3<T> GetPoint(const Line<T>& line, const T& t) {
		return line.r0 + line.d * t;
	}

	template<typename T>
	Math::vec3<T> GetPoint(const T& t, const Line<T>& line) {
		return line.r0 + line.d * t;
	}

	template<typename T>
	struct Plane {
		Math::vec3<T> r0;
		Math::vec3<T> n;
	};

	template<typename T>
	T Intersection(const Line<T>& line, const Plane<T>& plane) {
		return (Math::dot(plane.n, plane.r0) - Math::dot(plane.n, line.r0)) / Math::dot(plane.n, line.d);
	}

	template<typename T>
	T Intersection(const Plane<T>& plane, const Line<T>& line) {
		return (Math::dot(plane.n, plane.r0) - Math::dot(plane.n, line.r0)) / Math::dot(plane.n, line.d);
	}

}

typedef Math::Line<float> LineF;
typedef Math::Plane<float> PlaneF;

#endif