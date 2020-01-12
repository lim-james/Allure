#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Math.hpp"
#include "Vectors.hpp"

namespace Math {

	//  0  4  8 12
	//  1  5  9 13
	//  2  6 10 14
	//  3  7 11 15
	template<typename T>
	struct mat4 {
		T a[16] = { 0 };

		mat4() {};
		mat4(const T& i) {
			a[0] = a[5] = a[10] = a[15] = i;
		};
		mat4(const T& a0, const T& a1, const T& a2, const T& a3,
			 const T& a4, const T& a5, const T& a6, const T& a7,
			 const T& a8, const T& a9, const T& a10, const T& a11,
			 const T& a12, const T& a13, const T& a14, const T& a15) {
			a[0] = a0, a[1] = a1, a[2] = a2, a[3] = a3;
			a[4] = a4, a[5] = a5, a[6] = a6, a[7] = a7;
			a[8] = a8, a[9] = a9, a[10] = a10, a[11] = a11;
			a[12] = a12, a[13] = a13, a[14] = a14, a[15] = a15;
		};

		inline T& operator[](const unsigned& i) {
			return a[i];
		}

		inline const T& operator[](const unsigned& i) const {
			return a[i];
		}
	};

	// standard maths
	// mat4 + mat4 = mat4
	template<typename T>
	inline mat4<T> operator+(const mat4<T>& lhs, const mat4<T>& rhs) {
		mat4<T> result;
		for (unsigned i = 0; i < 16; ++i)
			result[i] = lhs[i] + rhs[i];
		return result;
	}
	// mat4 - mat4 = mat4
	template<typename T>
	inline mat4<T> operator-(const mat4<T>& lhs, const mat4<T>& rhs) {
		mat4<T> result;
		for (unsigned i = 0; i < 16; ++i)
			result[i] = lhs[i] - rhs[i];
		return result;
	}
	// mat4 * mat4 = mat4
	template<typename T>
	inline mat4<T> operator*(const mat4<T>& lhs, const mat4<T>& rhs) {
		mat4<T> result;

		for (unsigned y = 0; y < 4; ++y) {
			for (unsigned x = 0; x < 4; ++x) {
				result[y * 4 + x] =
					lhs[     x] * rhs[y * 4] +
					lhs[4  + x] * rhs[y * 4 + 1] +
					lhs[8  + x] * rhs[y * 4 + 2] +
					lhs[12 + x] * rhs[y * 4 + 3];
			}
		}

		return result;
	}
	// mat4 * vec3 = mat3
	template<typename T>
	inline vec3<T> operator*(const mat4<T>& lhs, const vec3<T>& rhs) {
		vec3<T> result(
			rhs.x * lhs[0] + rhs.y * lhs[4] + rhs.z * lhs[8] + lhs[12],
			rhs.x * lhs[1] + rhs.y * lhs[5] + rhs.z * lhs[9] + lhs[13],
			rhs.x * lhs[2] + rhs.y * lhs[6] + rhs.z * lhs[10] + lhs[14]
		);

		return result;
	}
	// mat4 * vec4 = vec4
	template<typename T>
	inline vec4<T> operator*(const mat4<T>& lhs, const vec4<T>& rhs) {
		vec4<T> result(
			rhs.x * lhs[0] + rhs.y * lhs[4] + rhs.z * lhs[8] + rhs.w * lhs[12],
			rhs.x * lhs[1] + rhs.y * lhs[5] + rhs.z * lhs[9] + rhs.w * lhs[13],
			rhs.x * lhs[2] + rhs.y * lhs[6] + rhs.z * lhs[10] + rhs.w * lhs[14],
			rhs.x * lhs[3] + rhs.y * lhs[7] + rhs.z * lhs[11] + rhs.w * lhs[15]
		);

		return result;
	}
	// mat4 * T = mat4
	template<typename T>
	inline mat4<T> operator*(const mat4<T>& lhs, const T& rhs) {
		mat4<T> result;
		for (unsigned i = 0; i < 16; ++i)
			result[i] = lhs[i] * rhs;
		return result;
	}
	// T * mat4 = mat4
	template<typename T>
	inline mat4<T> operator*(const T& lhs, const mat4<T>& rhs) {
		mat4<T> result;
		for (unsigned i = 0; i < 16; ++i)
			result[i] = lhs * rhs[i];
		return result;
	}
	// mat4 / T = mat4
	template<typename T>
	inline mat4<T> operator/(const mat4<T>& lhs, const T& rhs) {
		mat4<T> result;
		for (unsigned i = 0; i < 16; ++i)
			result[i] = lhs[i] / rhs;
		return result;
	}
	// shorthand operators
	// += mat4
	template<typename T>
	mat4<T>& operator+=(mat4<T>& lhs, const mat4<T>& rhs) {
		for (unsigned i = 0; i < 16; ++i)
			lhs[i] += rhs[i];
		return lhs;
	}
	// -= mat4
	template<typename T>
	mat4<T>& operator-=(mat4<T>& lhs, const mat4<T>& rhs) {
		for (unsigned i = 0; i < 16; ++i)
			lhs[i] -= rhs[i];
		return lhs;
	}
	// *= mat4
	template<typename T>
	mat4<T>& operator*=(mat4<T>& lhs, const mat4<T>& rhs) {
		mat4<T> result;
		for (unsigned y = 0; y < 4; ++y) {
			for (unsigned x = 0; x < 4; ++x) {
				result[y * 4 + x] =
					lhs[     x] * rhs[y * 4] +
					lhs[4  + x] * rhs[y * 4 + 1] +
					lhs[8  + x] * rhs[y * 4 + 2] +
					lhs[12 + x] * rhs[y * 4 + 3];
			}
		}

		for (unsigned i = 0; i < 16; ++i)
			lhs[i] = result[i];

		return lhs;
	}
	// *= T
	template<typename T>
	mat4<T>& operator*=(mat4<T>& lhs, const T& rhs) {
		for (unsigned i = 0; i < 16; ++i)
			lhs[i] *= rhs;
		return lhs;
	}
	// /= T
	template<typename T>
	mat4<T>& operator/=(mat4<T>& lhs, const T& rhs) {
		for (unsigned i = 0; i < 16; ++i)
			lhs[i] /= rhs;
		return lhs;
	}
	// unary operators
	template<typename T>
	inline mat4<T> operator-(mat4<T>& mat) {
		mat4<T> result;
		for (unsigned i = 0; i < 16; ++i)
			result = -mat[i];
		return result;
	}

	template<typename T>
	void Transpose(mat4<T>& mat) {
		for (unsigned y = 0; y < 4; y++)
			for (unsigned x = y + 1; x < 4; x++)
				std::swap(mat[y * 4 + x], mat[x * 4 + y]);
	}

	// set mat4f to zero
	template<typename T>
	void SetToZero(mat4<T>& mat) {
		mat[0] = mat[1] = mat[2] = mat[3] = 0;
		mat[4] = mat[5] = mat[6] = mat[7] = 0;
		mat[8] = mat[9] = mat[10] = mat[11] = 0;
		mat[12] = mat[13] = mat[14] = mat[15] = 0;
	}

	// set mat4f to identity
	template<typename T>
	void SetToIdentity(mat4<T>& mat) {
		SetToZero(mat);
		mat[0] = mat[5] = mat[10] = mat[15] = 1;
	}	
};

typedef Math::mat4<float> mat4f;

#endif