#include "VectorOperators.h"

// math operators

// vector + vector
// vector + T
// T + vector
// += vector
// += T

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator+(vec<size, T> const& lhs, vec<size, T> const& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs[i] + rhs[i];
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator+(vec<size, T> const& lhs, T const& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs[i] + rhs;
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator+(T const& lhs, vec<size, T> const& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs + rhs[i];
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T>& Math::operator+=(vec<size, T>& lhs, vec<size, T> const& rhs) {
	for (unsigned i = 0; i < size; ++i)
		lhs[i] += rhs[i];
	return lhs;
}

template<unsigned size, typename T>
inline Math::vec<size, T>& Math::operator+=(vec<size, T>& lhs, T const& rhs) {
	for (unsigned i = 0; i < size; ++i)
		lhs[i] += rhs;
	return lhs;
}

// vector - vector
// vector - T
// T - vector
// -= vector
// -= T

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator-(vec<size, T> const& lhs, vec<size, T> const& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs[i] - rhs[i];
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator-(vec<size, T> const& lhs, T const& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs[i] - rhs;
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator-(T const& lhs, vec<size, T> const& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs - rhs[i];
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T>& Math::operator-=(vec<size, T>& lhs, vec<size, T> const& rhs) {
	for (unsigned i = 0; i < size; ++i)
		lhs[i] -= rhs[i];
	return lhs;
}

template<unsigned size, typename T>
inline Math::vec<size, T>& Math::operator-=(vec<size, T>& lhs, T const& rhs) {
	for (unsigned i = 0; i < size; ++i)
		lhs[i] -= rhs;
	return lhs;
}

// vector * vector
// vector * T
// T * vector
// *= vector
// *= T

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator*(vec<size, T> const& lhs, vec<size, T> const& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs[i] * rhs[i];
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator*(vec<size, T> const& lhs, T const& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs[i] * rhs;
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator*(T const& lhs, vec<size, T> const& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs * rhs[i];
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T>& Math::operator*=(vec<size, T>& lhs, vec<size, T> const& rhs) {
	for (unsigned i = 0; i < size; ++i)
		lhs[i] *= rhs[i];
	return lhs;
}

template<unsigned size, typename T>
inline Math::vec<size, T>& Math::operator*=(vec<size, T>& lhs, T const& rhs) {
	for (unsigned i = 0; i < size; ++i)
		lhs[i] *= rhs;
	return lhs;
}

// division
// vector / vector
// vector / T
// T / vector
// /= vector
// /= T

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator/(vec<size, T> const& lhs, vec<size, T> const& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs[i] / rhs[i];
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator/(vec<size, T> const& lhs, T const& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs[i] / rhs;
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator/(T const& lhs, vec<size, T> const& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs / rhs[i];
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T>& Math::operator/=(vec<size, T>& lhs, vec<size, T> const& rhs) {
	for (unsigned i = 0; i < size; ++i)
		lhs[i] /= rhs[i];
	return lhs;
}

template<unsigned size, typename T>
inline Math::vec<size, T>& Math::operator/=(vec<size, T>& lhs, T const& rhs) {
	for (unsigned i = 0; i < size; ++i)
		lhs[i] /= rhs;
	return lhs;
}

// unary operator
// negate

template<unsigned size, typename T>
Math::vec<size, T> Math::operator-(vec<size, T> const& lhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = -lhs[i];
	return result;
}

// comparison operator
// equals
// not equals

template<unsigned size, typename T>
inline bool Math::operator==(vec<size, T> const& lhs, vec<size, T> const& rhs) {
	for (unsigned i = 0; i < size; ++i)
		if (lhs[i] != rhs[i]) return false;

	return true;
}

template<unsigned size, typename T>
inline bool Math::operator!=(vec<size, T> const& lhs, vec<size, T> const& rhs) {
	for (unsigned i = 0; i < size; ++i)
		if (lhs[i] != rhs[i]) return true;

	return false;
}

// io operator
// << & >>

template<unsigned size, typename T>
std::ostream& Math::operator<<(std::ostream& os, vec<size, T> const& v) {
	os << v[0];
	for (unsigned i = 1; i < size; ++i)
		os << ',' << v[i];
	return os;
}

template<unsigned size, typename T>
std::istream& Math::operator>>(std::istream& is, vec<size, T>& v) {
	unsigned index = 0;

	for (T i; is >> i;) {
		v[index++] = i;

		if (is.peek() == ',')
			is.ignore();
	}

	for (int i = index; i < size; ++i)
		v[i] = static_cast<T>(0);

	return is;
}

// vector methods
// absolute
// length
// length squared
// normalize
// normalized
// dot
// project

template<unsigned size, typename T>
inline Math::vec<size, T> Math::Abs(vec<size, T> const& v) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = abs(v[i]);
	return result;
}

template<unsigned size, typename T>
inline T Math::Length(Math::vec<size, T> const& v) {
	T sum = static_cast<T>(0);
	for (unsigned i = 0; i < size; ++i)
		sum += v[i] * v[i];
	return sqrt(sum);
}

template<unsigned size, typename T>
inline T Math::LengthSquared(Math::vec<size, T> const& v) {
	T sum = static_cast<T>(0);
	for (unsigned i = 0; i < size; ++i)
		sum += v[i] * v[i];
	return sum;
}

template<unsigned size, typename T>
inline void Math::Normalize(vec<size, T>& lhs) {
	const T length = Length(lhs);
	if (length) lhs /= length;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::Normalized(vec<size, T> const& lhs) {
	const T length = Length(lhs);
	return length ? lhs / length : lhs;
}

template<unsigned size, typename T>
inline T Math::Dot(vec<size, T> const& lhs, vec<size, T> const& rhs) {
	T sum = static_cast<T>(0);
	for (unsigned i = 0; i < size; ++i)
		sum += lhs[i] * rhs[i];
	return sum;
}

template<typename T>
Math::vec<3, T> Math::Cross(vec<3, T> const& lhs, vec<3, T> const& rhs) {
	return vec<3, T>(
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x
		);
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::Project(vec<size, T> const& v, vec<size, T> const& onto) {
	return Dot(v, onto) / LengthSquared(onto) * onto;
}
