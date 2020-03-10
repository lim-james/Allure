#include "vec2.h"

// custom constructors 

template<typename T>
Math::vec<2, T>::vec(const T& i) : x(i), y(i) {}
template<typename T>
Math::vec<2, T>::vec(const T& x, const T& y) : x(x), y(y) {}

template<typename T> 
template<typename t>
Math::vec<2, T>::vec(const vec<2, t>& v) 
	: x(static_cast<T>(v.x))
	, y(static_cast<T>(v.y)) {}

template<typename T> 
template<unsigned s>
Math::vec<2, T>::vec(const vec<s, T>& v) {
	if (s == 1) {
		x = v[0];
		y = static_cast<T>(0);
	} else {
		x = v[0];
		y = v[1];
	}
}

// member operators
// = operator
// [] operator

template<typename T>
Math::vec<2, T>& Math::vec<2, T>::operator=(const T& v) {
	x = v;
	y = v;
	return *this;
}

template<typename T>
Math::vec<2, T>& Math::vec<2, T>::operator=(const vec<2, T>&  v) {
	x = v.x;
	y = v.y;
	return *this;
}

template<typename T>
inline T& Math::vec<2, T>::operator[](const unsigned& i) {
	return data[i];
}

template<typename T>
inline const T& Math::vec<2, T>::operator[](const unsigned& i) const {
	return data[i];
}
