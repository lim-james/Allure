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
	inline vec<size, T> operator+(vec<size, T> const& lhs, vec<size, T> const& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator+(vec<size, T> const& lhs, T const& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator+(T const& lhs, vec<size, T> const& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator+=(vec<size, T>& lhs, vec<size, T> const& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator+=(vec<size, T>& lhs, T const& rhs);

	// vector - vector
	// vector - T
	// T - vector
	// -= vector
	// -= T

	template<unsigned size, typename T>
	inline vec<size, T> operator-(vec<size, T> const& lhs, vec<size, T> const& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator-(vec<size, T> const& lhs, T const& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator-(T const& lhs, vec<size, T> const& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator-=(vec<size, T>& lhs, vec<size, T> const& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator-=(vec<size, T>& lhs, T const& rhs);

	// vector * vector
	// vector * T
	// T * vector
	// *= vector
	// *= T

	template<unsigned size, typename T>
	inline vec<size, T> operator*(vec<size, T> const& lhs, vec<size, T> const& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator*(vec<size, T> const& lhs, T const& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator*(T const& lhs, vec<size, T> const& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator*=(vec<size, T>& lhs, vec<size, T> const& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator*=(vec<size, T>& lhs, T const& rhs);

	// division
	// vector / vector
	// vector / T
	// T / vector
	// /= vector
	// /= T

	template<unsigned size, typename T>
	inline vec<size, T> operator/(vec<size, T> const& lhs, vec<size, T> const& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator/(vec<size, T> const& lhs, T const& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator/(T const& lhs, vec<size, T> const& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator/=(vec<size, T>& lhs, vec<size, T> const& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator/=(vec<size, T>& lhs, T const& rhs);

	// unary operator
	// negate

	template<unsigned size, typename T>
	inline vec<size, T> operator-(vec<size, T> const& lhs);

	// comparison operator
	// equals
	// not equals

	template<unsigned size, typename T>
	inline bool operator==(vec<size, T> const& lhs, vec<size, T> const& rhs);
	template<unsigned size, typename T>
	inline bool operator!=(vec<size, T> const& lhs, vec<size, T> const& rhs);

	// io operator
	// << & >>

	template<unsigned size, typename T>
	std::ostream& operator<<(std::ostream& os, vec<size, T> const& v);
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
	inline vec<size, T> Abs(vec<size, T> const& v);
	// length
	template<unsigned size, typename T>
	inline T Length(vec<size, T> const& v);
	template<unsigned size, typename T>
	inline T LengthSquared(vec<size, T> const& v);
	// normalize
	template<unsigned size, typename T>
	inline void Normalize(vec<size, T>& lhs);
	template<unsigned size, typename T>
	inline vec<size, T> Normalized(vec<size, T> const& lhs);
	// dot
	template<unsigned size, typename T>
	inline T Dot(vec<size, T> const& lhs, vec<size, T> const& rhs);
	// cross
	template<typename T>
	inline vec<3, T> Cross(vec<3, T> const& lhs, vec<3, T> const& rhs);
	// project
	template<unsigned size, typename T>
	inline vec<size, T> Project(vec<size, T> const& v, vec<size, T> const& onto);

}

#include "VectorOperators.inl"

#endif
