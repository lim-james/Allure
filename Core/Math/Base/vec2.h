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
		vec(const vec<2, T>&) = default;

		// custom constructors

		vec(const T& i = static_cast<T>(0));
		vec(const T& x, const T& y);
		// differing types
		template<typename t>
		vec(const vec<2, t>& v);
		// differing sizes
		template<unsigned s>
		vec(const vec<s, T>& v);
		
		// member operators
		// = operator
		// [] operator

		vec<2, T>& operator=(vec<2, T>&&) = default;
		vec<2, T>& operator=(const T& v);
		vec<2, T>& operator=(const vec<2, T>&  v);

		inline T& operator[](const unsigned& i);
		inline const T& operator[](const unsigned& i) const;
	};

}

#include "vec2.inl"

#endif
