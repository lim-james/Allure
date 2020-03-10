#ifndef VECTOR_OPERATORS_H
#define VECTOR_OPERATORS_H

#include "BaseTypes.h"

#include <iostream>

namespace Math {
	// math operators

	// vector + vector
	// vector + T
	// T + vector
	// += vector
	// += T
	template<unsigned size, typename T>
	inline vec<size, T> operator+(const vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator+(const vec<size, T>& lhs, const T& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator+(const T& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator+=(vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator+=(vec<size, T>& lhs, const T& rhs);

	// vector - vector
	// vector - T
	// T - vector
	// -= vector
	// -= T

	template<unsigned size, typename T>
	inline vec<size, T> operator-(const vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator-(const vec<size, T>& lhs, const T& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator-(const T& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator-=(vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator-=(vec<size, T>& lhs, const T& rhs);

	// vector * vector
	// vector * T
	// T * vector
	// *= vector
	// *= T

	template<unsigned size, typename T>
	inline vec<size, T> operator*(const vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator*(const vec<size, T>& lhs, const T& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator*(const T& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator*=(vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator*=(vec<size, T>& lhs, const T& rhs);

	// division
	// vector / vector
	// vector / T
	// T / vector
	// /= vector
	// /= T

	template<unsigned size, typename T>
	inline vec<size, T> operator/(const vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator/(const vec<size, T>& lhs, const T& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator/(const T& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator/=(vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator/=(vec<size, T>& lhs, const T& rhs);

	// unary operator
	// negate

	template<unsigned size, typename T>
	inline vec<size, T> operator-(const vec<size, T>& lhs);

	// comparison operator
	// equals
	// not equals

	template<unsigned size, typename T>
	inline bool operator==(const vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline bool operator!=(const vec<size, T>& lhs, const vec<size, T>& rhs);

	// io operator
	// << & >>

	template<unsigned size, typename T>
	std::ostream& operator<<(std::ostream& os, const vec<size, T>& v);
	template<unsigned size, typename T>
	std::istream& operator>>(std::istream& is, vec<size, T>& v);

	// vector methods
	// absolute
	// length
	// length squared
	// normalize
	// normalized
	// dot
	// project

	template<unsigned size, typename T>
	inline vec<size, T> Abs(const vec<size, T>& v);
	// length
	template<unsigned size, typename T>
	inline T Length(const vec<size, T>& v);
	template<unsigned size, typename T>
	inline T LengthSquared(const vec<size, T>& v);
	// normalize
	template<unsigned size, typename T>
	inline void Normalize(vec<size, T>& lhs);
	template<unsigned size, typename T>
	inline vec<size, T> Normalized(const vec<size, T>& lhs);
	// dot
	template<unsigned size, typename T>
	inline T Dot(const vec<size, T>& lhs, const vec<size, T>& rhs);
	// cross
	template<typename T>
	inline vec<3, T> Cross(const vec<3, T>& lhs, const vec<3, T>& rhs);
	// project
	template<unsigned size, typename T>
	inline vec<size, T> Project(const vec<size, T>& v, const vec<size, T>& onto);

}

#include "VectorOperators.inl"

#endif
