#include "VectorOperators.h"

// math operators

// vector + vector
// vector + T
// T + vector
// += vector
// += T

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator+(const vec<size, T>& lhs, const vec<size, T>& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs[i] + rhs[i];
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator+(const vec<size, T>& lhs, const T& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs[i] + rhs;
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator+(const T& lhs, const vec<size, T>& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs + rhs[i];
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T>& Math::operator+=(vec<size, T>& lhs, const vec<size, T>& rhs) {
	for (unsigned i = 0; i < size; ++i)
		lhs[i] += rhs[i];
	return lhs;
}

template<unsigned size, typename T>
inline Math::vec<size, T>& Math::operator+=(vec<size, T>& lhs, const T& rhs) {
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
inline Math::vec<size, T> Math::operator-(const vec<size, T>& lhs, const vec<size, T>& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs[i] - rhs[i];
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator-(const vec<size, T>& lhs, const T& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs[i] - rhs;
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator-(const T& lhs, const vec<size, T>& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs - rhs[i];
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T>& Math::operator-=(vec<size, T>& lhs, const vec<size, T>& rhs) {
	for (unsigned i = 0; i < size; ++i)
		lhs[i] -= rhs[i];
	return lhs;
}

template<unsigned size, typename T>
inline Math::vec<size, T>& Math::operator-=(vec<size, T>& lhs, const T& rhs) {
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
inline Math::vec<size, T> Math::operator*(const vec<size, T>& lhs, const vec<size, T>& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs[i] * rhs[i];
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator*(const vec<size, T>& lhs, const T& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs[i] * rhs;
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator*(const T& lhs, const vec<size, T>& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs * rhs[i];
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T>& Math::operator*=(vec<size, T>& lhs, const vec<size, T>& rhs) {
	for (unsigned i = 0; i < size; ++i)
		lhs[i] *= rhs[i];
	return lhs;
}

template<unsigned size, typename T>
inline Math::vec<size, T>& Math::operator*=(vec<size, T>& lhs, const T& rhs) {
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
inline Math::vec<size, T> Math::operator/(const vec<size, T>& lhs, const vec<size, T>& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs[i] / rhs[i];
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator/(const vec<size, T>& lhs, const T& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs[i] / rhs;
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::operator/(const T& lhs, const vec<size, T>& rhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = lhs / rhs[i];
	return result;
}

template<unsigned size, typename T>
inline Math::vec<size, T>& Math::operator/=(vec<size, T>& lhs, const vec<size, T>& rhs) {
	for (unsigned i = 0; i < size; ++i)
		lhs[i] /= rhs[i];
	return lhs;
}

template<unsigned size, typename T>
inline Math::vec<size, T>& Math::operator/=(vec<size, T>& lhs, const T& rhs) {
	for (unsigned i = 0; i < size; ++i)
		lhs[i] /= rhs;
	return lhs;
}

// unary operator
// negate

template<unsigned size, typename T>
Math::vec<size, T> Math::operator-(const vec<size, T>& lhs) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = -lhs[i];
	return result;
}

// comparison operator
// equals
// not equals

template<unsigned size, typename T>
inline bool Math::operator==(const vec<size, T>& lhs, const vec<size, T>& rhs) {
	for (unsigned i = 0; i < size; ++i)
		if (lhs[i] != rhs[i]) return false;

	return true;
}

template<unsigned size, typename T>
inline bool Math::operator!=(const vec<size, T>& lhs, const vec<size, T>& rhs) {
	for (unsigned i = 0; i < size; ++i)
		if (lhs[i] != rhs[i]) return true;

	return false;
}

// io operator
// << & >>

template<unsigned size, typename T>
std::ostream& Math::operator<<(std::ostream& os, const vec<size, T>& v) {
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
inline Math::vec<size, T> Math::Abs(const vec<size, T>& v) {
	vec<size, T> result;
	for (unsigned i = 0; i < size; ++i)
		result[i] = abs(v[i]);
	return result;
}

template<unsigned size, typename T>
inline T Math::Length(const Math::vec<size, T>& v) {
	T sum = static_cast<T>(0);
	for (unsigned i = 0; i < size; ++i)
		sum += v[i] * v[i];
	return sqrt(sum);
}

template<unsigned size, typename T>
inline T Math::LengthSquared(const Math::vec<size, T>& v) {
	T sum = static_cast<T>(0);
	for (unsigned i = 0; i < size; ++i)
		sum += v[i] * v[i];
	return sum;
}

template<unsigned size, typename T>
inline void Math::Normalize(vec<size, T>& lhs) {
	const T magnitude = Length(lhs);
	lhs /= magnitude;
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::Normalized(const vec<size, T>& lhs) {
	return lhs / Length(lhs);
}

template<unsigned size, typename T>
inline T Math::Dot(const vec<size, T>& lhs, const vec<size, T>& rhs) {
	T sum = static_cast<T>(0);
	for (unsigned i = 0; i < size; ++i)
		sum += lhs[i] * rhs[i];
	return sum;
}

template<typename T>
Math::vec<3, T> Math::Cross(const vec<3, T>& lhs, const vec<3, T>& rhs) {
	return vec<3, T>(
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x
		);
}

template<unsigned size, typename T>
inline Math::vec<size, T> Math::Project(const vec<size, T>& v, const vec<size, T>& onto) {
	return Dot(v, onto) / LengthSquared(onto) * onto;
}
