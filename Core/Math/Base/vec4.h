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
			struct { vec<3, T> xyz; T w; };
			struct { T x; vec<3, T> yzw; };

			struct { T r, g, b, a; };
			struct { T r; vec<2, T> gb; };
			struct { vec<2, T> rg, ba; };
			struct { vec<3, T> rgb; T a; };
			struct { T r; vec<3, T> gba; };

			struct { vec<2, T> origin, size; };
		};

		// default constructors
		vec(vec<4, T>&&) = default;
		vec(vec<4, T> const&) = default;

		// custom constructors

		vec(T const& i = static_cast<T>(0));
		vec(T const& x, T const& y, T const& z, T const& w);
		// joining vectors
		vec(T const& x, vec<3, T> const& yzw);
		vec(vec<3, T> const& xyz, T const& w);
		vec(vec<2, T> const& xy, vec<2, T> const& zw);
		vec(vec<2, T> const& xy, T const& z, T const& w);
		vec(T const& x, vec<2, T> const& yz, T const& w);
		vec(T const& x, T const& y, vec<2, T> const& zw);
		// differing types
		template<typename t>
		vec(vec<4, t> const& v);
		// differing sizes
		template<unsigned s>
		vec(vec<s, T> const& v);

		// member operators
		// = operator
		// [] operator

		vec<4, T>& operator=(vec<4, T>&&) = default;
		vec<4, T>& operator=(T const& v);
		vec<4, T>& operator=(vec<4, T> const& v);

		inline T& operator[](unsigned const& i);
		inline T const& operator[](unsigned const& i) const;
	};

}

#include "vec4.inl"

#endif
