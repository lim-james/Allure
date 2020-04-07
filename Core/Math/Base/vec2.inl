#include "vec2.h"

// custom constructors 

template<typename T>
Math::vec<2, T>::vec(T const& i) : x(i), y(i) {}
template<typename T>
Math::vec<2, T>::vec(T const& x, T const& y) : x(x), y(y) {}

template<typename T> 
template<typename t>
Math::vec<2, T>::vec(vec<2, t> const& v) 
	: x(static_cast<T>(v.x))
	, y(static_cast<T>(v.y)) {}

template<typename T> 
template<unsigned s>
Math::vec<2, T>::vec(vec<s, T> const& v) {
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
Math::vec<2, T>& Math::vec<2, T>::operator=(T const& v) {
	x = v;
	y = v;
	return *this;
}

template<typename T>
Math::vec<2, T>& Math::vec<2, T>::operator=(vec<2, T> const& v) {
	x = v.x;
	y = v.y;
	return *this;
}

template<typename T>
inline T& Math::vec<2, T>::operator[](unsigned const& i) {
	return data[i];
}

template<typename T>
inline T const& Math::vec<2, T>::operator[](unsigned const& i) const {
	return data[i];
}
