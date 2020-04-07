#include "vec3.h"

// custom constructors 

template<typename T>
Math::vec<3, T>::vec(T const& i) : x(i), y(i), z(i) {}
template<typename T>
Math::vec<3, T>::vec(T const& x, T const& y, T const& z) : x(x), y(y), z(z) {}
template<typename T>
Math::vec<3, T>::vec(T const& x, vec<2, T> const& yz) : x(x), yz(yz) {}
template<typename T>
Math::vec<3, T>::vec(vec<2, T> const& xy, T const& z) : xy(xy), z(z) {}

template<typename T> 
template<typename t>
Math::vec<3, T>::vec(vec<3, t> const& v)
	: x(static_cast<T>(v.x))
	, y(static_cast<T>(v.y))
	, z(static_cast<T>(v.z)) {}

template<typename T> 
template<unsigned s>
Math::vec<3, T>::vec(vec<s, T> const& v) {
	if (s == 1) {
		x = v[0];
		y = z = static_cast<T>(0);
	} else if (s == 2) {
		x = v[0];
		y = v[1];
		z = static_cast<T>(0);
	} else {
		x = v[0];
		y = v[1];
		z = v[2];
	}
}

// member operators
// = operator
// [] operator

template<typename T>
Math::vec<3, T>& Math::vec<3, T>::operator=(T const& v) {
	x = v;
	y = v;
	z = v;
	return *this;
}

template<typename T>
Math::vec<3, T>& Math::vec<3, T>::operator=(vec<3, T> const& v) {
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

template<typename T>
inline T& Math::vec<3, T>::operator[](unsigned const& i) {
	return data[i];
}

template<typename T>
inline T const& Math::vec<3, T>::operator[](unsigned const& i) const {
	return data[i];
}
