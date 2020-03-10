#include "Vectors.h"

#include <string>

// default constructor

template<unsigned size, typename T>
Math::vec<size, T>::vec() {
	SetZero();
}

// default destructor

template<unsigned size, typename T>
Math::vec<size, T>::~vec() {}

// custom constructors

template<unsigned size, typename T>
template <typename ... Arguments>
Math::vec<size, T>::vec(Arguments ... arguments) {
	Set(arguments...);
}

template<unsigned size, typename T>
template<unsigned s>
Math::vec<size, T>::vec(const vec<s, T>& v) {
	Set(v);
}

template<unsigned size, typename T>
template<unsigned s1, unsigned s2>
Math::vec<size, T>::vec(const vec<s1, T>& v1, const vec<s2, T>& v2) {
	Set(v1, v2);
}

template<unsigned size, typename T>
template<typename t>
Math::vec<size, T>::vec(const vec<size, t>& v) {
	Set(v);
}

// member operators
// = operator
// [] operator

template<unsigned size, typename T>
Math::vec<size, T>& Math::vec<size, T>::operator=(const T& v) {
	for (unsigned i = 0; i < size; ++i)
		data[i] = v;
	return *this;
}

template<unsigned size, typename T>
Math::vec<size, T>& Math::vec<size, T>::operator=(const vec<size, T>&  v) {
	for (unsigned i = 0; i < size; ++i)
		data[i] = v[i];
	return *this;
}

template<unsigned size, typename T>
inline T& Math::vec<size, T>::operator[](const unsigned& i) {
	return data[i];
}

template<unsigned size, typename T>
inline const T& Math::vec<size, T>::operator[](const unsigned& i) const {
	return data[i];
}

// setters
// set zero
// join vectors
// set values

template<unsigned size, typename T>
void Math::vec<size, T>::SetZero() {
	for (unsigned i = 0; i < size; ++i)
		data[i] = static_cast<T>(0);
}

template<unsigned size, typename T>
template<typename ...Arguments>
void Math::vec<size, T>::Set(Arguments ...arguments) {
	//static_assert(sizeof...(arguments) == size, "Missing arguments.");
	static_assert(sizeof...(arguments) == size || sizeof...(arguments) == 1, "Missing arguments.");

	unsigned index = 0;
	T temp[] = { (data[index++] = arguments, static_cast<T>(0))... };

	if (index == 1) {
		for (unsigned i = 1; i < size; ++i)
			data[i] = data[0];
	}
}

template<unsigned size, typename T>
template<unsigned s>
void Math::vec<size, T>::Set(const vec<s, T>& v) {
	if (s < size) {
		for (unsigned i = 0; i < s; ++i)
			data[i] = v.data[i];

		for (unsigned i = s; i < size; ++i)
			data[i] = static_cast<T>(0);
	} else {
		for (unsigned i = 0; i < size; ++i)
			data[i] = v.data[i];
	}
}

template<unsigned size, typename T>
template<unsigned s1, unsigned s2>
void Math::vec<size, T>::Set(const vec<s1, T>& v1, const vec<s2, T>& v2) {
	if (s1 < size) {
		for (unsigned i = 0; i < s1; ++i)
			data[i] = v1.data[i];

		const int r = size - s1;

		if (s2 < r) {
			for (unsigned i = 0; i < s2; ++i)
				data[i + s1] = v2.data[i];

			for (unsigned i = 0; i < r - s2; ++i)
				data[i + r] = 0;
		} else {
			for (unsigned i = 0; i < r; ++i)
				data[i + s1] = v2.data[i];
		}
	} else {
		for (unsigned i = 0; i < size; ++i)
			data[i] = v1.data[i];
	}
}

template<unsigned size, typename T>
template<typename t>
void Math::vec<size, T>::Set(const vec<size, t>& v) {
	for (unsigned i = 0; i < size; ++i)
		data[i] = static_cast<T>(v.data[i]);
}

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

	for (int i; is >> i;) {
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

