#ifndef VECTORS_H
#define VECTORS_H

#include <iostream>

namespace Math {
	template<unsigned size, typename T>
	struct vec {
		union {
			struct {
				T data[size];
			};

			struct {
				T u, v;
			};

			struct {
				T width, height, depth;
			};

			struct {
				T x, y, z, w;
			};

			struct {
				T r, g, b, a;
			};
		};

		// default constructors
		vec();
		vec(vec<size, T>&&) = default;
		vec(const vec<size, T>&) = default;

		// default destructor
		~vec();

		// custom constructors

		template <typename ... Arguments>
		vec(Arguments ... arguments);
		template<unsigned s>
		vec(const vec<s, T>& v);
		template<unsigned s1, unsigned s2>
		vec(const vec<s1, T>& v1, const vec<s2, T>& v2);
		template<typename t>
		vec(const vec<size, t>& v);

		// member operators
		// = operator
		// [] operator

		vec<size, T>& operator=(vec<size, T>&&) = default;
		vec<size, T>& operator=(const T& v);
		vec<size, T>& operator=(const vec<size, T>&  v);
		inline T& operator[](const unsigned& i);
		inline const T& operator[](const unsigned& i) const;

		// setters
		// set zero
		// join vectors
		// set values

		void SetZero();
		template <typename ... Arguments>
		void Set(Arguments ... arguments);
		template<unsigned s>
		void Set(const vec<s, T>& v);
		template<unsigned s1, unsigned s2>
		void Set(const vec<s1, T>& v1, const vec<s2, T>& v2);
		// cast
		template<typename t>
		void Set(const vec<size, t>& v);

	};

	// math operators

	// vector + vector
	// vector + T
	// T + vector
	// += vector
	// += T
	template<unsigned size, typename T>
	inline vec<size, T> operator+(const vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator+(const vec<size, T>& lhs, const T& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator+(const T& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator+=(vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator+=(vec<size, T>& lhs, const T& rhs);

	// vector - vector
	// vector - T
	// T - vector
	// -= vector
	// -= T

	template<unsigned size, typename T>
	inline vec<size, T> operator-(const vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator-(const vec<size, T>& lhs, const T& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator-(const T& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator-=(vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator-=(vec<size, T>& lhs, const T& rhs);

	// vector * vector
	// vector * T
	// T * vector
	// *= vector
	// *= T

	template<unsigned size, typename T>
	inline vec<size, T> operator*(const vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator*(const vec<size, T>& lhs, const T& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator*(const T& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator*=(vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator*=(vec<size, T>& lhs, const T& rhs);

	// division
	// vector / vector
	// vector / T
	// T / vector
	// /= vector
	// /= T
	
	template<unsigned size, typename T>
	inline vec<size, T> operator/(const vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator/(const vec<size, T>& lhs, const T& rhs);
	template<unsigned size, typename T>
	inline vec<size, T> operator/(const T& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator/=(vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline vec<size, T>& operator/=(vec<size, T>& lhs, const T& rhs);

	// unary operator
	// negate

	template<unsigned size, typename T>
	inline vec<size, T> operator-(const vec<size, T>& lhs);

	// comparison operator
	// equals
	// not equals

	template<unsigned size, typename T>
	inline bool operator==(const vec<size, T>& lhs, const vec<size, T>& rhs);
	template<unsigned size, typename T>
	inline bool operator!=(const vec<size, T>& lhs, const vec<size, T>& rhs);

	// io operator
	// << & >>

	template<unsigned size, typename T>
	std::ostream& operator<<(std::ostream& os, const vec<size, T>& v);
	template<unsigned size, typename T>
	std::istream& operator>>(std::istream& is, vec<size, T>& v);

	// vector methods
	// absolute
	// length
	// length squared
	// normalize
	// normalized
	// dot
	// project

	template<unsigned size, typename T>
	inline vec<size, T> Abs(const vec<size, T>& v);
	// length
	template<unsigned size, typename T>
	inline T Length(const vec<size, T>& v);
	template<unsigned size, typename T>
	inline T LengthSquared(const vec<size, T>& v);
	// normalize
	template<unsigned size, typename T>
	inline void Normalize(vec<size, T>& lhs);
	template<unsigned size, typename T>
	inline vec<size, T> Normalized(const vec<size, T>& lhs);
	// dot
	template<unsigned size, typename T>
	inline T Dot(const vec<size, T>& lhs, const vec<size, T>& rhs);
	// cross
	template<typename T>
	inline vec<3, T> Cross(const vec<3, T>& lhs, const vec<3, T>& rhs);
	// project
	template<unsigned size, typename T>
	inline vec<size, T> Project(const vec<size, T>& v, const vec<size, T>& onto);

}

typedef Math::vec<2, int> vec2i;
typedef Math::vec<3, int> vec3i;
typedef Math::vec<4, int> vec4i;

typedef Math::vec<2, unsigned> vec2u;
typedef Math::vec<3, unsigned> vec3u;
typedef Math::vec<4, unsigned> vec4u;

typedef Math::vec<2, float> vec2f;
typedef Math::vec<3, float> vec3f;
typedef Math::vec<4, float> vec4f;

typedef Math::vec<2, double> vec2d;
typedef Math::vec<3, double> vec3d;
typedef Math::vec<4, double> vec4d;

#include "Vectors.inl"

#endif
