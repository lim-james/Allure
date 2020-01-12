#ifndef MATH_HPP
#define MATH_HPP

#include <utility>

namespace Math {

	const float	PI = 3.14159f;
	const float	HALF_PI = 1.5708f;

	template<typename T>
	inline T Rad(const T& deg) {
		return deg * PI / 180.f;
	}

	template<typename T>
	inline T Deg(const T& rad) {
		return rad * 180.f / PI;
	}

	template<typename T>
	inline T Max(const T& a, const T& b) {
		return a > b ? a : b;
	}

	template<typename T>
	inline T Min(const T& a, const T& b) {
		return a < b ? a : b;
	}

	template<typename T>
	T Clamp(const T& a, const T& lowerBound, const T& upperBound) {
		if (a < lowerBound) return lowerBound;
		if (a > upperBound) return upperBound;
		return a;
	}

	template<typename T>
	T Round(const T& value, const int& segments) {
		const T s = static_cast<T>(segments);
		return round(value * s) / s;
	}

	template<typename T, typename M>
	T Round(const T& value, const int& segments, const M& range) {
		const T v = value / range;
		const T s = static_cast<T>(segments);
		return range * round(v * s) / s;
	}

	template<typename T>
	T Abs(const T& a) {
		return a < 0 ? -a : a;
	}

	template<typename T>
	std::pair<T, T> Quadratic(const T& a, const T& b, const T& c) {
		const T& d = b * b - 4 * a * c;

		if (d < 0)
			return {};

		return std::make_pair(
			(-b + sqrt(d)) / (2 * a),
			(-b - sqrt(d)) / (2 * a)
		);
	}

	template<typename T>
	bool InRange(const T& value, const T& min, const T& max) {
		return value >= min && value <= max;
	}

}

#endif