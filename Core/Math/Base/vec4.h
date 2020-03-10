#ifndef VEC_4_H
#define VEC_4_H

#include "BaseTypes.h"

namespace Math {

	template<typename T>
	struct vec<4, T> {
		union {
			T data[4];

			struct { T x, y, z, w; };
			struct { T x; vec<2, T> yz; };
			struct { vec<2, T> xy, zw; };
			struct { vec<3, T> xyz; };
			struct { T x; vec<3, T> yzw; };

			struct { T r, g, b, a; };
			struct { T r; vec<2, T> gb; };
			struct { vec<2, T> rg, ba; };
			struct { vec<3, T> rgb; };
			struct { T r; vec<3, T> gba; };

			struct { vec<2, T> origin, size; };
		};

		// default constructors
		vec(vec<4, T>&&) = default;
		vec(const vec<4, T>&) = default;

		// custom constructors

		vec(const T& i = static_cast<T>(0));
		vec(const T& x, const T& y, const T& z, const T& w);
		// joining vectors
		vec(const T& x, const vec<3, T>& yzw);
		vec(const vec<3, T>& xyz, const T& w);
		vec(const vec<2, T>& xy, const vec<2, T>& zw);
		vec(const vec<2, T>& xy, const T& z, const T& w);
		vec(const T& x, const vec<2, T>& yz, const T& w);
		vec(const T& x, const T& y, const vec<2, T>& zw);
		// differing types
		template<typename t>
		vec(const vec<4, t>& v);
		// differing sizes
		template<unsigned s>
		vec(const vec<s, T>& v);

		// member operators
		// = operator
		// [] operator

		vec<4, T>& operator=(vec<4, T>&&) = default;
		vec<4, T>& operator=(const T& v);
		vec<4, T>& operator=(const vec<4, T>&  v);

		inline T& operator[](const unsigned& i);
		inline const T& operator[](const unsigned& i) const;
	};

}

#include "vec4.inl"

#endif
