#ifndef MATH_HPP
#define MATH_HPP

#include <utility>

namespace Math {

	const float	PI = 3.14159f;
	const float	HALF_PI = 1.5708f;

	const float toRad = PI / 180.0f;
	const float toDeg = 180.0f / PI;

	template<typename T>
	inline T Max(T const& a, T const& b) {
		return a > b ? a : b;
	}

	template<typename T>
	inline T Min(T const& a, T const& b) {
		return a < b ? a : b;
	}

	template<typename T>
	inline T Mix(T const& a, T const& b, float const& amount) {
		return amount * a + (1.f - amount) * b;
	}

	template<typename T>
	T Clamp(T const& a, T const& lowerBound, T const& upperBound) {
		if (a < lowerBound) return lowerBound;
		if (a > upperBound) return upperBound;
		return a;
	}

	template<typename T>
	T Round(T const& value, int const& segments) {
		const T s = static_cast<T>(segments);
		return round(value * s) / s;
	}

	template<typename T, typename M>
	T Round(T const& value, int const& segments, M const& range) {
		const T v = value / range;
		const T s = static_cast<T>(segments);
		return range * round(v * s) / s;
	}

	template<typename T>
	T Abs(T const& a) {
		return a < 0 ? -a : a;
	}

	template<typename T>
	std::pair<T, T> Quadratic(T const& a, T const& b, T const& c) {
		T const& d = b * b - 4 * a * c;

		if (d < 0)
			return {};

		return std::make_pair(
			(-b + sqrt(d)) / (2 * a),
			(-b - sqrt(d)) / (2 * a)
		);
	}

	template<typename T>
	bool InRange(T const& value, T const& min, T const& max) {
		return value >= min && value <= max;
	}

	template<typename T>
	T Lerp(T const& from, T const& to, float const& t) {
		return from + (to - from) * t;
	}

}

#endif