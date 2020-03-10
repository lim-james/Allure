#include "vec3.h"

// custom constructors 

template<typename T>
Math::vec<3, T>::vec(const T& i) : x(i), y(i), z(i) {}
template<typename T>
Math::vec<3, T>::vec(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}
template<typename T>
Math::vec<3, T>::vec(const T & x, const vec<2, T>& yz) : x(x), yz(yz) {}
template<typename T>
Math::vec<3, T>::vec(const vec<2, T>& xy, const T & z) : xy(xy), z(z) {}

template<typename T> 
template<typename t>
Math::vec<3, T>::vec(const vec<3, t>& v)
	: x(static_cast<T>(v.x))
	, y(static_cast<T>(v.y))
	, z(static_cast<T>(v.z)) {}

template<typename T> 
template<unsigned s>
Math::vec<3, T>::vec(const vec<s, T>& v) {
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
Math::vec<3, T>& Math::vec<3, T>::operator=(const T& v) {
	x = v;
	y = v;
	z = v;
	return *this;
}

template<typename T>
Math::vec<3, T>& Math::vec<3, T>::operator=(const vec<3, T>&  v) {
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

template<typename T>
inline T& Math::vec<3, T>::operator[](const unsigned& i) {
	return data[i];
}

template<typename T>
inline const T& Math::vec<3, T>::operator[](const unsigned& i) const {
	return data[i];
}
