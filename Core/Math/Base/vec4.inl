#include "vec4.h"

// custom constructors 

template<typename T>
Math::vec<4, T>::vec(const T& i) : x(i), y(i), z(i), w(i) {}
template<typename T>
Math::vec<4, T>::vec(const T& x, const T& y, const T& z, const T& w) : x(x), y(y), z(z), w(w) {}
template<typename T>
Math::vec<4, T>::vec(const T& x, const vec<3, T>& yzw) : x(x), yzw(yzw) { }
template<typename T>
Math::vec<4, T>::vec(const vec<3, T>& xyz, const T & w) : xyz(xyz), w(w) {}
template<typename T>
Math::vec<4, T>::vec(const vec<2, T>& xy, const vec<2, T>& zw) : xy(xy), zw(zw) {}
template<typename T>
Math::vec<4, T>::vec(const vec<2, T>& xy, const T& z, const T& w) : xy(xy), z(z), w(w) {}
template<typename T>
Math::vec<4, T>::vec(const T& x, const vec<2, T>& yz, const T& y) : x(x), yz(yz), w(w) {}
template<typename T>
Math::vec<4, T>::vec(const T& x, const T& y, const vec<2, T>& zw) : x(x), y(y), zw(zw) {}

template<typename T>
template<typename t>
Math::vec<4, T>::vec(const vec<4, t>& v) 
	: x(static_cast<T>(v.x))
	, y(static_cast<T>(v.y))
	, z(static_cast<T>(v.z))
	, w(static_cast<T>(v.w)) {}

template<typename T>
template<unsigned s>
Math::vec<4, T>::vec(const vec<s, T>& v) {
	if (s == 1) {
		x = v[0];
		y = z = w = static_cast<T>(0);
	} else if (s == 2) {
		x = v[0];
		y = v[1];
		z = w = static_cast<T>(0);
	} else  if (s == 3) {
		x = v[0];
		y = v[1];
		z = v[2];
		w = static_cast<T>(0);
	} else {
		x = v[0];
		y = v[1];
		z = v[2];
		w = v[3];
	}
}

// member operators
// = operator
// [] operator

template<typename T>
Math::vec<4, T>& Math::vec<4, T>::operator=(const T& v) {
	x = y = z = w = v;
	return *this;
}

template<typename T>
Math::vec<4, T>& Math::vec<4, T>::operator=(const vec<4, T>&  v) {
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

template<typename T>
inline T& Math::vec<4, T>::operator[](const unsigned& i) {
	return data[i];
}

template<typename T>
inline const T& Math::vec<4, T>::operator[](const unsigned& i) const {
	return data[i];
}
