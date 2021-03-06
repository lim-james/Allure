#ifndef MAT4_HPP
#define MAT4_HPP

#include "Math.hpp"
#include "Vectors.h"

namespace Math {

	// 0 4 8 12
	// 1 5 9 13
	// 2 6 10 14
	// 3 7 11 15
	template<typename T>
	struct mat4 {
		T a[16] = { 0 };

		mat4() {};
		mat4(T const& i) {
			a[0] = a[5] = a[10] = a[15] = i;
		};
		mat4(T const& a0, T const& a1, T const& a2, T const& a3,
			 T const& a4, T const& a5, T const& a6, T const& a7,
			 T const& a8, T const& a9, T const& a10, T const& a11,
			 T const& a12, T const& a13, T const& a14, T const& a15) {
			a[0] = a0, a[1] = a1, a[2] = a2, a[3] = a3;
			a[4] = a4, a[5] = a5, a[6] = a6, a[7] = a7;
			a[8] = a8, a[9] = a9, a[10] = a10, a[11] = a11;
			a[12] = a12, a[13] = a13, a[14] = a14, a[15] = a15;
		};

		inline T& operator[](unsigned const& i) {
			return a[i];
		}

		inline T const& operator[](unsigned const& i) const {
			return a[i];
		}
	};

	// standard maths
	// mat4 + mat4 = mat4
	template<typename T>
	inline mat4<T> operator+(mat4<T> const& lhs, mat4<T> const& rhs) {
		mat4<T> result;
		for (unsigned i = 0; i < 16; ++i)
			result[i] = lhs[i] + rhs[i];
		return result;
	}
	// mat4 - mat4 = mat4
	template<typename T>
	inline mat4<T> operator-(mat4<T> const& lhs, mat4<T> const& rhs) {
		mat4<T> result;
		for (unsigned i = 0; i < 16; ++i)
			result[i] = lhs[i] - rhs[i];
		return result;
	}
	// mat4 * mat4 = mat4
	template<typename T>
	inline mat4<T> operator*(mat4<T> const& lhs, mat4<T> const& rhs) {
		mat4<T> result;

		for (unsigned y = 0; y < 4; ++y) {
			for (unsigned x = 0; x < 4; ++x) {
				result[y * 4 + x] =
					lhs[   x] * rhs[y * 4] +
					lhs[4 + x] * rhs[y * 4 + 1] +
					lhs[8 + x] * rhs[y * 4 + 2] +
					lhs[12 + x] * rhs[y * 4 + 3];
			}
		}

		return result;
	}
	// mat4 * vec3 = mat3
	template<typename T>
	inline vec<3, T> operator*(mat4<T> const& lhs, vec<3, T> const& rhs) {
		vec<3, T> result(
			rhs.x * lhs[0] + rhs.y * lhs[4] + rhs.z * lhs[8] + lhs[12],
			rhs.x * lhs[1] + rhs.y * lhs[5] + rhs.z * lhs[9] + lhs[13],
			rhs.x * lhs[2] + rhs.y * lhs[6] + rhs.z * lhs[10] + lhs[14]
		);

		return result;
	}
	// mat4 * vec4 = vec4
	template<typename T>
	inline vec<4, T> operator*(mat4<T> const& lhs, vec<4, T> const& rhs) {
		vec<4, T> result(
			rhs.x * lhs[0] + rhs.y * lhs[4] + rhs.z * lhs[8] + rhs.w * lhs[12],
			rhs.x * lhs[1] + rhs.y * lhs[5] + rhs.z * lhs[9] + rhs.w * lhs[13],
			rhs.x * lhs[2] + rhs.y * lhs[6] + rhs.z * lhs[10] + rhs.w * lhs[14],
			rhs.x * lhs[3] + rhs.y * lhs[7] + rhs.z * lhs[11] + rhs.w * lhs[15]
		);

		return result;
	}
	// mat4 * T = mat4
	template<typename T>
	inline mat4<T> operator*(mat4<T> const& lhs, T const& rhs) {
		mat4<T> result;
		for (unsigned i = 0; i < 16; ++i)
			result[i] = lhs[i] * rhs;
		return result;
	}
	// T * mat4 = mat4
	template<typename T>
	inline mat4<T> operator*(T const& lhs, mat4<T> const& rhs) {
		mat4<T> result;
		for (unsigned i = 0; i < 16; ++i)
			result[i] = lhs * rhs[i];
		return result;
	}
	// mat4 / T = mat4
	template<typename T>
	inline mat4<T> operator/(mat4<T> const& lhs, T const& rhs) {
		mat4<T> result;
		for (unsigned i = 0; i < 16; ++i)
			result[i] = lhs[i] / rhs;
		return result;
	}
	// shorthand operators
	// += mat4
	template<typename T>
	mat4<T>& operator+=(mat4<T>& lhs, mat4<T> const& rhs) {
		for (unsigned i = 0; i < 16; ++i)
			lhs[i] += rhs[i];
		return lhs;
	}
	// -= mat4
	template<typename T>
	mat4<T>& operator-=(mat4<T>& lhs, mat4<T> const& rhs) {
		for (unsigned i = 0; i < 16; ++i)
			lhs[i] -= rhs[i];
		return lhs;
	}
	// *= mat4
	template<typename T>
	mat4<T>& operator*=(mat4<T>& lhs, mat4<T> const& rhs) {
		mat4<T> result;
		for (unsigned y = 0; y < 4; ++y) {
			for (unsigned x = 0; x < 4; ++x) {
				result[y * 4 + x] =
					lhs[   x] * rhs[y * 4] +
					lhs[4 + x] * rhs[y * 4 + 1] +
					lhs[8 + x] * rhs[y * 4 + 2] +
					lhs[12 + x] * rhs[y * 4 + 3];
			}
		}

		for (unsigned i = 0; i < 16; ++i)
			lhs[i] = result[i];

		return lhs;
	}
	// *= T
	template<typename T>
	mat4<T>& operator*=(mat4<T>& lhs, T const& rhs) {
		for (unsigned i = 0; i < 16; ++i)
			lhs[i] *= rhs;
		return lhs;
	}
	// /= T
	template<typename T>
	mat4<T>& operator/=(mat4<T>& lhs, T const& rhs) {
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

	// transpose mat4
	template<typename T>
	void Transpose(mat4<T>& mat) {
		for (unsigned y = 0; y < 4; y++)
			for (unsigned x = y + 1; x < 4; x++)
				std::swap(mat[y * 4 + x], mat[x * 4 + y]);
	}

	// set mat4 to zero
	template<typename T>
	void SetToZero(mat4<T>& mat) {
		mat[0] = mat[1] = mat[2] = mat[3] = 0;
		mat[4] = mat[5] = mat[6] = mat[7] = 0;
		mat[8] = mat[9] = mat[10] = mat[11] = 0;
		mat[12] = mat[13] = mat[14] = mat[15] = 0;
	}

	// set mat4 to identity
	template<typename T>
	void SetToIdentity(mat4<T>& mat) {
		SetToZero(mat);
		mat[0] = mat[5] = mat[10] = mat[15] = 1;
	}	

	// inverse mat4 
	template<typename T>
	void Inverse(mat4<T>& mat) {
		T inv[16], det;

		inv[0] = mat[5] * mat[10] * mat[15] -
			mat[5] * mat[11] * mat[14] -
			mat[9] * mat[6] * mat[15] +
			mat[9] * mat[7] * mat[14] +
			mat[13] * mat[6] * mat[11] -
			mat[13] * mat[7] * mat[10];

		inv[4] = -mat[4] * mat[10] * mat[15] +
			mat[4] * mat[11] * mat[14] +
			mat[8] * mat[6] * mat[15] -
			mat[8] * mat[7] * mat[14] -
			mat[12] * mat[6] * mat[11] +
			mat[12] * mat[7] * mat[10];

		inv[8] = mat[4] * mat[9] * mat[15] -
			mat[4] * mat[11] * mat[13] -
			mat[8] * mat[5] * mat[15] +
			mat[8] * mat[7] * mat[13] +
			mat[12] * mat[5] * mat[11] -
			mat[12] * mat[7] * mat[9];

		inv[12] = -mat[4] * mat[9] * mat[14] +
			mat[4] * mat[10] * mat[13] +
			mat[8] * mat[5] * mat[14] -
			mat[8] * mat[6] * mat[13] -
			mat[12] * mat[5] * mat[10] +
			mat[12] * mat[6] * mat[9];

		inv[1] = -mat[1] * mat[10] * mat[15] +
			mat[1] * mat[11] * mat[14] +
			mat[9] * mat[2] * mat[15] -
			mat[9] * mat[3] * mat[14] -
			mat[13] * mat[2] * mat[11] +
			mat[13] * mat[3] * mat[10];

		inv[5] = mat[0] * mat[10] * mat[15] -
			mat[0] * mat[11] * mat[14] -
			mat[8] * mat[2] * mat[15] +
			mat[8] * mat[3] * mat[14] +
			mat[12] * mat[2] * mat[11] -
			mat[12] * mat[3] * mat[10];

		inv[9] = -mat[0] * mat[9] * mat[15] +
			mat[0] * mat[11] * mat[13] +
			mat[8] * mat[1] * mat[15] -
			mat[8] * mat[3] * mat[13] -
			mat[12] * mat[1] * mat[11] +
			mat[12] * mat[3] * mat[9];

		inv[13] = mat[0] * mat[9] * mat[14] -
			mat[0] * mat[10] * mat[13] -
			mat[8] * mat[1] * mat[14] +
			mat[8] * mat[2] * mat[13] +
			mat[12] * mat[1] * mat[10] -
			mat[12] * mat[2] * mat[9];

		inv[2] = mat[1] * mat[6] * mat[15] -
			mat[1] * mat[7] * mat[14] -
			mat[5] * mat[2] * mat[15] +
			mat[5] * mat[3] * mat[14] +
			mat[13] * mat[2] * mat[7] -
			mat[13] * mat[3] * mat[6];

		inv[6] = -mat[0] * mat[6] * mat[15] +
			mat[0] * mat[7] * mat[14] +
			mat[4] * mat[2] * mat[15] -
			mat[4] * mat[3] * mat[14] -
			mat[12] * mat[2] * mat[7] +
			mat[12] * mat[3] * mat[6];

		inv[10] = mat[0] * mat[5] * mat[15] -
			mat[0] * mat[7] * mat[13] -
			mat[4] * mat[1] * mat[15] +
			mat[4] * mat[3] * mat[13] +
			mat[12] * mat[1] * mat[7] -
			mat[12] * mat[3] * mat[5];

		inv[14] = -mat[0] * mat[5] * mat[14] +
			mat[0] * mat[6] * mat[13] +
			mat[4] * mat[1] * mat[14] -
			mat[4] * mat[2] * mat[13] -
			mat[12] * mat[1] * mat[6] +
			mat[12] * mat[2] * mat[5];

		inv[3] = -mat[1] * mat[6] * mat[11] +
			mat[1] * mat[7] * mat[10] +
			mat[5] * mat[2] * mat[11] -
			mat[5] * mat[3] * mat[10] -
			mat[9] * mat[2] * mat[7] +
			mat[9] * mat[3] * mat[6];

		inv[7] = mat[0] * mat[6] * mat[11] -
			mat[0] * mat[7] * mat[10] -
			mat[4] * mat[2] * mat[11] +
			mat[4] * mat[3] * mat[10] +
			mat[8] * mat[2] * mat[7] -
			mat[8] * mat[3] * mat[6];

		inv[11] = -mat[0] * mat[5] * mat[11] +
			mat[0] * mat[7] * mat[9] +
			mat[4] * mat[1] * mat[11] -
			mat[4] * mat[3] * mat[9] -
			mat[8] * mat[1] * mat[7] +
			mat[8] * mat[3] * mat[5];

		inv[15] = mat[0] * mat[5] * mat[10] -
			mat[0] * mat[6] * mat[9] -
			mat[4] * mat[1] * mat[10] +
			mat[4] * mat[2] * mat[9] +
			mat[8] * mat[1] * mat[6] -
			mat[8] * mat[2] * mat[5];

		det = mat[0] * inv[0] + mat[1] * inv[4] + mat[2] * inv[8] + mat[3] * inv[12];

		if (det == 0)
			return;

		det = static_cast<T>(1) / det;

		for (int i = 0; i < 16; ++i)
			mat[i] = inv[i] * det;
	}

	// return inversed mat4 
	template<typename T>
	mat4<T> Inversed(mat4<T>& mat) {
		mat4<T> result = mat;
		Inverse(result);
		return result;
	}
};

typedef Math::mat4<float> mat4f;

#endif