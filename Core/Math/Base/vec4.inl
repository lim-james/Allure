#include "vec4.h"

// custom constructors 

template<typename T>
Math::vec<4, T>::vec(T const& i) : x(i), y(i), z(i), w(i) {}
template<typename T>
Math::vec<4, T>::vec(T const& x, T const& y, T const& z, T const& w) : x(x), y(y), z(z), w(w) {}
template<typename T>
Math::vec<4, T>::vec(T const& x, vec<3, T> const& yzw) : x(x), yzw(yzw) { }
template<typename T>
Math::vec<4, T>::vec(vec<3, T> const& xyz, T const& w) : xyz(xyz), w(w) {}
template<typename T>
Math::vec<4, T>::vec(vec<2, T> const& xy, vec<2, T> const& zw) : xy(xy), zw(zw) {}
template<typename T>
Math::vec<4, T>::vec(vec<2, T> const& xy, T const& z, T const& w) : xy(xy), zw(z, w) {}
template<typename T>
Math::vec<4, T>::vec(T const& x, vec<2, T> const& yz, T const& y) : x(x), yz(yz), w(w) {}
template<typename T>
Math::vec<4, T>::vec(T const& x, T const& y, vec<2, T> const& zw) : x(x), y(y), zw(zw) {}

template<typename T>
template<typename t>
Math::vec<4, T>::vec(vec<4, t> const& v) 
	: x(static_cast<T>(v.x))
	, y(static_cast<T>(v.y))
	, z(static_cast<T>(v.z))
	, w(static_cast<T>(v.w)) {}

template<typename T>
template<unsigned s>
Math::vec<4, T>::vec(vec<s, T> const& v) {
	if (s == 1) {
		x = v[0];
		y = z = w = static_cast<T>(0);
	} else if (s == 2) {
		x = v[0];
		y = v[1];
		z = w = static_cast<T>(0);
	} else if (s == 3) {
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
Math::vec<4, T>& Math::vec<4, T>::operator=(T const& v) {
	x = y = z = w = v;
	return *this;
}

template<typename T>
Math::vec<4, T>& Math::vec<4, T>::operator=(vec<4, T> const& v) {
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

template<typename T>
inline T& Math::vec<4, T>::operator[](unsigned const& i) {
	return data[i];
}

template<typename T>
inline T const& Math::vec<4, T>::operator[](unsigned const& i) const {
	return data[i];
}
