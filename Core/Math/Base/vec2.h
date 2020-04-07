#ifndef VEC_2_H
#define VEC_2_H

#include "BaseTypes.h"

namespace Math {

	template<typename T>
	struct vec<2, T> {
		union {
			T data[2];

			struct { T x, y; };
			struct { T r, g; };
			struct { T u, v; };
			struct { T w, h; };
		};

		// default constructors
		vec(vec<2, T>&&) = default;
		vec(vec<2, T> const&) = default;

		// custom constructors

		vec(T const& i = static_cast<T>(0));
		vec(T const& x, T const& y);
		// differing types
		template<typename t>
		vec(vec<2, t> const& v);
		// differing sizes
		template<unsigned s>
		vec(vec<s, T> const& v);
		
		// member operators
		// = operator
		// [] operator

		vec<2, T>& operator=(vec<2, T>&&) = default;
		vec<2, T>& operator=(T const& v);
		vec<2, T>& operator=(vec<2, T> const& v);

		inline T& operator[](unsigned const& i);
		inline T const& operator[](unsigned const& i) const;
	};

}

#include "vec2.inl"

#endif
