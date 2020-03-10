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
		vec(const vec<3, T>&) = default;

		// custom constructors

		vec(const T& i = static_cast<T>(0));
		vec(const T& x, const T& y, const T& z);
		// joining vectors
		vec(const T& x, const vec<2, T>& yz);
		vec(const vec<2, T>& xy, const T& z);
		// differing types
		template<typename t>
		vec(const vec<3, t>& v);
		// differing sizes
		template<unsigned s>
		vec(const vec<s, T>& v);

		// member operators
		// = operator
		// [] operator

		vec<3, T>& operator=(vec<3, T>&&) = default;
		vec<3, T>& operator=(const T& v);
		vec<3, T>& operator=(const vec<3, T>&  v);

		inline T& operator[](const unsigned& i);
		inline const T& operator[](const unsigned& i) const;
	};

}

#include "vec3.inl"

#endif
