#ifndef VEC_3_H
#define VEC_3_H

#include "BaseTypes.h"

namespace Math {

	template<typename T>
	struct vec<3, T> {
		union {
			T data[3];

			struct { T x, y, z; };
			struct { vec<2, T> xy; };
			struct { T x; vec<2, T> yz; };

			struct { T r, g, b; };
			struct { vec<2, T> rg; };
			struct { T r; vec<2, T> gb; };
		};

		// default constructors
		vec(vec<3, T>&&) = default;
		vec(vec<3, T> const&) = default;

		// custom constructors

		vec(T const& i = static_cast<T>(0));
		vec(T const& x, T const& y, T const& z);
		// joining vectors
		vec(T const& x, vec<2, T> const& yz);
		vec(vec<2, T> const& xy, T const& z);
		// differing types
		template<typename t>
		vec(vec<3, t> const& v);
		// differing sizes
		template<unsigned s>
		vec(vec<s, T> const& v);

		// member operators
		// = operator
		// [] operator

		vec<3, T>& operator=(vec<3, T>&&) = default;
		vec<3, T>& operator=(T const& v);
		vec<3, T>& operator=(vec<3, T> const& v);

		inline T& operator[](unsigned const& i);
		inline T const& operator[](unsigned const& i) const;
	};

}

#include "vec3.inl"

#endif
