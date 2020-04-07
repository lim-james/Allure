#ifndef MAT4_TRANSFORM_HPP
#define MAT4_TRANSFORM_HPP

#include "Mat4.hpp"

namespace Math {

	template<typename T>
	mat4<T> Frustum(T const& left, T const& right, T const& bottom, T const& top, T const& nearPlane, T const& farPlane) {
		return mat4<T>(
			2.f * nearPlane / (right - left), 0, 0, 0,
			0, 2.f * nearPlane / (top - bottom), 0, 0,
			(right + left) / (right - left), (top + bottom) / (top - bottom), -(farPlane + nearPlane) / (farPlane - nearPlane), -1,
			0, 0, -2.f * farPlane * nearPlane / (farPlane - nearPlane), 0
		);
	}

	template<typename T>
	mat4<T> Orthographic(T const& left, T const& right, T const& bottom, T const& top, T const& nearPlane, T const& farPlane) {
		return mat4<T>(
			2.f / (right - left), 0, 0, 0,
			0, 2.f / (top - bottom), 0, 0,
			0, 0, -2.f / (farPlane - nearPlane), 0,
			-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(farPlane + nearPlane) / (farPlane - nearPlane), 1
		);
	}

	template<typename T>
	mat4<T> Perspective(T const& fovy, T const& aspect, T const& nearPlane, T const& farPlane) {
		T f = 1.f / tan(Rad(fovy / 2.f));
		return mat4<T>(
			f / aspect, 0, 0, 0,
			0,	f, 0, 0,
			0, 0, (farPlane + nearPlane) / (nearPlane - farPlane), -1,
			0, 0, 2.f * farPlane * nearPlane / (nearPlane - farPlane), 0
		);
	}

	template<typename T>
	void SetToScale(mat4<T>& mat, vec<3, T> const& vec) {
		SetToIdentity(mat);
		mat[0] = vec.x;
		mat[5] = vec.y;
		mat[10] = vec.z;
	}

	template<typename T>
	void Scale(mat4<T>& mat, vec<3, T> const& vec) {
		mat4<T> transform;
		SetToScale(transform, vec);
		mat *= transform;
	}

	template<typename T>
	mat4<T> Scaled(mat4<T> const& mat, vec<3, T> const& vec) {
		mat4<T> transform;
		SetToScale(transform, vec);
		return mat * transform;
	}

	template<typename T>
	void SetToRotation(mat4<T>& mat, T const& angle, vec<3, T> const& axis) {
		SetToIdentity(mat);

		const T mag = sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);

		const T x = axis.x / mag, y = axis.y / mag, z = axis.z / mag;
		const T c = cos(Rad(angle)), s = sin(Rad(angle));

		mat[0] = x * x * (1 - c) + c;
		mat[1] = y * x * (1 - c) + z * s;
		mat[2] = x * z * (1 - c) - y * s;
		mat[3] = 0;
		mat[4] = x * y * (1 - c) - z * s;
		mat[5] = y * y * (1 - c) + c;
		mat[6] = y * z * (1 - c) + x * s;
		mat[7] = 0;
		mat[8] = x * z * (1 - c) + y * s;
		mat[9] = y * z * (1 - c) - x * s;
		mat[10] = z * z * (1 - c) + c;
		mat[11] = 0;
		mat[12] = 0;
		mat[13] = 0;
		mat[14] = 0;
		mat[15] = 1;
	}

	template<typename T>
	void Rotate(mat4<T>& mat, T const& angle, vec<3, T> const& axis) {
		mat4<T> transform;
		SetToRotation(transform, angle, axis);
		mat *= transform;
	}

	template<typename T>
	mat4<T> Rotated(mat4<T> const& mat, T const& angle, vec<3, T> const& axis) {
		mat4<T> transform;
		SetToRotation(transform, angle, axis);
		return mat * transform;
	}

	template<typename T>
	void SetToRotation(mat4<T>& mat, vec<3, T> const& angles) {
		SetToIdentity(mat);
		Rotate(mat, angles.y, vec3f(0.f, 1.f, 0.f));
		Rotate(mat, angles.x, vec3f(1.f, 0.f, 0.f));
		Rotate(mat, angles.z, vec3f(0.f, 0.f, 1.f));
	}

	template<typename T>
	void Rotate(mat4<T>& mat, vec<3, T> const& angles) {
		mat4<T> transform;
		SetToRotation(transform, angles);
		mat *= transform;
	}

	template<typename T>
	mat4<T> Rotated(mat4<T> const& mat, vec<3, T> const& angles) {
		mat4<T> transform;
		SetToRotation(transform, angles);
		return mat * transform;
	}

	template<typename T>
	void SetToTranslation(mat4<T>& mat, vec<3, T> const& vec) {
		SetToIdentity(mat);
		mat[12] = vec.x;
		mat[13] = vec.y;
		mat[14] = vec.z;
	}

	template<typename T>
	void Translate(mat4<T>& mat, vec<3, T> const& vec) {
		mat4<T> transform;
		SetToTranslation(transform, vec);
		mat *= transform;
	}

	template<typename T>
	mat4<T> Translated(mat4<T> const& mat, vec<3, T> const& vec) {
		mat4<T> transform;
		SetToTranslation(transform, vec);
		return mat * transform;
	}

	template<typename T>
	void SetToTransform(mat4<T>& mat, vec<3, T> const& translation, vec<3, T> const& rotation, vec<3, T> const& scale) {
		SetToIdentity(mat);
		Translate(mat, translation);
		Rotate(mat, rotation);
		Scale(mat, scale);
	}

	template<typename T>
	mat4<T> LookAt(vec3f const& eye, vec<3, T> const& center, vec<3, T> const& up) {
		vec<3, T> f(center.x - eye.x, center.y - eye.y, center.z - eye.z);

		Normalize(f);

		const vec<3, T> s = Cross(f, Normalized(up));
		const vec<3, T> u = Cross(s, f);

		mat4<T> mat(
			s.x, u.x, -f.x, 0,
			s.y, u.y, -f.y, 0,
			s.z, u.z, -f.z, 0,
			0, 0, 0, 1
		);

		return Translated(mat, -eye);
	}

}

#endif