#ifndef VECTORS_NEW_H
#define VECTORS_NEW_H

#include <iostream>

namespace Math {
	template<unsigned size, typename T>
	struct Vector {

		union {
			struct {
				T data[size];
			};

			struct {
				T u, v;
			};

			struct {
				T x, y, z, w;
			};

			struct {
				T r, g, b, a;
			};
		};

		// default constructors

		Vector(Vector<size, T>&&) = default;
		Vector(const Vector<size, T>&) = default;

		// default destructor

		~Vector() {}

		// custom constructor

		Vector() {
			SetZero();
		}

		template<unsigned s>
		Vector(const Vector<s, T>& v) {
			Set(v);
		}

		template<unsigned s1, unsigned s2>
		Vector(const Vector<s1, T>& v1, const Vector<s2, T>& v2) {
			Set(v1, v2);
		}

		Vector(T v...) {
			va_list args;
			va_start(args, v);

			data[0] = v;

			for (unsigned i = 1; i < size; ++i) {
				const int a = va_arg(args, int);
				data[i] = static_cast<T>(a);
			}

			va_end(args);
		}

		// operators

		Vector<size, T>& operator=(Vector<size, T>&&) = default;

		Vector<size, T>& operator=(const T& v) {
			for (unsigned i = 0; i < size; ++i)
				data[i] = v;
			return *this;
		}

		Vector<size, T>& operator=(const Vector<size, T>&  v) {
			for (unsigned i = 0; i < size; ++i)
				data[i] = v[i];
			return *this;
		}

		inline T& operator[](const unsigned& i) {
			return data[i];
		}

		inline const T& operator[](const unsigned& i) const {
			return data[i];
		}

		// helper methods

		void SetZero() {
			for (unsigned i = 0; i < size; ++i)
				data[i] = static_cast<T>(0);
		}

		template<unsigned s>
		void Set(const Vector<s, T>& v) {
			if (s < size) {
				for (unsigned i = 0; i < s; ++i)
					data[i] = v.data[i];

				for (unsigned i = s; i < size; ++i)
					data[i] = static_cast<T>(0);
			} else {
				for (unsigned i = 0; i < size; ++i)
					data[i] = v.data[i];
			}
		}

		template<unsigned s1, unsigned s2>
		void Set(const Vector<s1, T>& v1, const Vector<s2, T>& v2) {
			if (s1 < size) {
				for (unsigned i = 0; i < s1; ++i)
					data[i] = v1.data[i];

				const int r = size - s1;

				if (s2 < r) {
					for (unsigned i = 0; i < s2; ++i)
						data[i + s1] = v2.data[i];

					for (unsigned i = 0; i < r - s2; ++i)
						data[i + r] = 0;
				} else {
					for (unsigned i = 0; i < r; ++i)
						data[i + s1] = v2.data[i];
				}
			} else {
				for (unsigned i = 0; i < size; ++i)
					data[i] = v1.data[i];
			}
		}

		void Set(T v, ...) {
			va_list args;
			va_start(args, v);

			data[0] = v;

			for (unsigned i = 1; i < size; ++i)
				data[i] = va_arg(args, T);

			va_end(args);
		}
	};

	// math operators

	// vector + vector
	// vector + T
	// T + vector
	// += vector
	// += T
	
	template<unsigned size, typename T>
	inline Vector<size, T> operator+(const Vector<size, T>& lhs, const Vector<size, T>& rhs) {
		Vector<size, T> result;
		for (unsigned i = 0; i < size; ++i)
			result[i] = lhs[i] + rhs[i];
		return result;
	}

	template<unsigned size, typename T>
	inline Vector<size, T> operator+(const Vector<size, T>& lhs, const T& rhs) {
		Vector<size, T> result;
		for (unsigned i = 0; i < size; ++i)
			result[i] = lhs[i] + rhs;
		return result;
	}

	template<unsigned size, typename T>
	inline Vector<size, T> operator+(const T& lhs, const Vector<size, T>& rhs) {
		Vector<size, T> result;
		for (unsigned i = 0; i < size; ++i)
			result[i] = lhs + rhs[i];
		return result;
	}

	template<unsigned size, typename T>
	inline Vector<size, T>& operator+=(Vector<size, T>& lhs, const Vector<size, T>& rhs) {
		for (unsigned i = 0; i < size; ++i)
			lhs[i] += rhs[i];
		return lhs;
	}
	
	template<unsigned size, typename T>
	inline Vector<size, T>& operator+=(Vector<size, T>& lhs, const T& rhs) {
		for (unsigned i = 0; i < size; ++i)
			lhs[i] += rhs;
		return lhs;
	}

	// vector - vector
	// vector - T
	// T - vector
	// -= vector
	// -= T

	template<unsigned size, typename T>
	inline Vector<size, T> operator-(const Vector<size, T>& lhs, const Vector<size, T>& rhs) {
		Vector<size, T> result;
		for (unsigned i = 0; i < size; ++i)
			result[i] = lhs[i] - rhs[i];
		return result;
	}

	template<unsigned size, typename T>
	inline Vector<size, T> operator-(const Vector<size, T>& lhs, const T& rhs) {
		Vector<size, T> result;
		for (unsigned i = 0; i < size; ++i)
			result[i] = lhs[i] - rhs;
		return result;
	}

	template<unsigned size, typename T>
	inline Vector<size, T> operator-(const T& lhs, const Vector<size, T>& rhs) {
		Vector<size, T> result;
		for (unsigned i = 0; i < size; ++i)
			result[i] = lhs - rhs[i];
		return result;
	}

	template<unsigned size, typename T>
	inline Vector<size, T>& operator-=(Vector<size, T>& lhs, const Vector<size, T>& rhs) {
		for (unsigned i = 0; i < size; ++i)
			lhs[i] -= rhs[i];
		return lhs;
	}

	template<unsigned size, typename T>
	inline Vector<size, T>& operator-=(Vector<size, T>& lhs, const T& rhs) {
		for (unsigned i = 0; i < size; ++i)
			lhs[i] -= rhs;
		return lhs;
	}

	// vector * vector
	// vector * T
	// T * vector
	// *= vector
	// *= T

	template<unsigned size, typename T>
	inline Vector<size, T> operator*(const Vector<size, T>& lhs, const Vector<size, T>& rhs) {
		Vector<size, T> result;
		for (unsigned i = 0; i < size; ++i)
			result[i] = lhs[i] * rhs[i];
		return result;
	}

	template<unsigned size, typename T>
	inline Vector<size, T> operator*(const Vector<size, T>& lhs, const T& rhs) {
		Vector<size, T> result;
		for (unsigned i = 0; i < size; ++i)
			result[i] = lhs[i] * rhs;
		return result;
	}

	template<unsigned size, typename T>
	inline Vector<size, T> operator*(const T& lhs, const Vector<size, T>& rhs) {
		Vector<size, T> result;
		for (unsigned i = 0; i < size; ++i)
			result[i] = lhs * rhs[i];
		return result;
	}

	template<unsigned size, typename T>
	inline Vector<size, T>& operator*=(Vector<size, T>& lhs, const Vector<size, T>& rhs) {
		for (unsigned i = 0; i < size; ++i)
			lhs[i] *= rhs[i];
		return lhs;
	}

	template<unsigned size, typename T>
	inline Vector<size, T>& operator*=(Vector<size, T>& lhs, const T& rhs) {
		for (unsigned i = 0; i < size; ++i)
			lhs[i] *= rhs;
		return lhs;
	}

	// division
	// vector / vector
	// vector / T
	// T / vector
	// /= vector
	// /= T
	
	template<unsigned size, typename T>
	inline Vector<size, T> operator/(const Vector<size, T>& lhs, const Vector<size, T>& rhs) {
		Vector<size, T> result;
		for (unsigned i = 0; i < size; ++i)
			result[i] = lhs[i] / rhs[i];
		return result;
	}
	
	template<unsigned size, typename T>
	inline Vector<size, T> operator/(const Vector<size, T>& lhs, const T& rhs) {
		Vector<size, T> result;
		for (unsigned i = 0; i < size; ++i)
			result[i] = lhs[i] / rhs;
		return result;
	}

	template<unsigned size, typename T>
	inline Vector<size, T> operator/(const T& lhs, const Vector<size, T>& rhs) {
		Vector<size, T> result;
		for (unsigned i = 0; i < size; ++i)
			result[i] = lhs / rhs[i];
		return result;
	}

	template<unsigned size, typename T>
	inline Vector<size, T>& operator/=(Vector<size, T>& lhs, const Vector<size, T>& rhs) {
		for (unsigned i = 0; i < size; ++i)
			lhs[i] /= rhs[i];
		return lhs;
	}

	template<unsigned size, typename T>
	inline Vector<size, T>& operator/=(Vector<size, T>& lhs, const T& rhs) {
		for (unsigned i = 0; i < size; ++i)
			lhs[i] /= rhs;
		return lhs;
	}

	// unary operator
	// negate
	template<unsigned size, typename T>
	inline Vector<size, T> operator-(const Vector<size, T>& lhs) {
		Vector<size, T> result;
		for (unsigned i = 0; i < size; ++i)
			result[i] = -lhs[i];
		return result;
	}

	// comparison operator
	// equals
	template<unsigned size, typename T>
	inline bool operator==(const Vector<size, T>& lhs, const Vector<size, T>& rhs) {
		for (unsigned i = 0; i < size; ++i)
			if (lhs[i] != rhs[i]) return false;

		return true;
	}
	// not equals
	template<unsigned size, typename T>
	inline bool operator!=(const Vector<size, T>& lhs, const Vector<size, T>& rhs) {
		for (unsigned i = 0; i < size; ++i)
			if (lhs[i] != rhs[i]) return true;

		return false;
	}

	// vector methods
	// length
	template<unsigned size, typename T>
	inline T Length(const Vector<size, T>& v) {
		T sum = static_cast<T>(0);
		for (unsigned i = 0; i < size; ++i)
			sum += v[i] * v[i];
		return sqrt(sum);
	}

	// length squared
	template<unsigned size, typename T>
	inline T LengthSquared(const Vector<size, T>& v) {
		T sum = static_cast<T>(0);
		for (unsigned i = 0; i < size; ++i)
			sum += v[i] * v[i];
		return sum;
	}

	// dot
	template<unsigned size, typename T>
	inline T LengthSquared(const Vector<size, T>& lhs, const Vector<size, T>& rhs) {
		T sum = static_cast<T>(0);
		for (unsigned i = 0; i < size; ++i)
			sum += lhs[i] * rhs[i];
		return sum;
	}

	// normalize
	template<unsigned size, typename T>
	inline void Normalize(Vector<size, T>& lhs) {
		const T magnitude = Length(lhs);
		lhs /= magnitude;
	}

	// normalized
	template<unsigned size, typename T>
	inline void Normalized(const Vector<size, T>& lhs) {
		return lhs / Length(lhs);
	}
	
	// project
	template<unsigned size, typename T>
	inline Vector<size, T> Project(const Vector<size, T>& v, const Vector<size, T>& onto) {
		return Dot(v, onto) / LengthSquared(onto) * onto;
	}

	// absolute
	template<unsigned size, typename T>
	inline Vector<size, T> Abs(const Vector<size, T>& v) {
		Vector<size, T> result;
		for (unsigned i = 0; i < size; ++i)
			result[i] = abs(v[i]);
		return result;
	}

	// output stream
	template<unsigned size, typename T>
	std::ostream& operator<<(std::ostream& os, const Vector<size, T>& v) {
		os << v[0];
		for (unsigned i = 1; i < size; ++i)
			os << ',' << v[i];
		return os;
	}


}

#endif
